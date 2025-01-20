#define _GNU_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "fmt.h"
#include "fmt/fmt_strm_internal.c"
#include "fmt/fmt_str.c"
#include "mmap/mmap_read.c"
#include "mmap/mmap_unmap.c"
#include "open/open_read.c"
#include "open/open_trunc.c"

// this program is a dependency generator
// we have two kinds of dependencies, libdep and dep
// libdep: "TAI_OBJS=tai_add.o tai_now.o tai_pack.o tai_sub.o tai_uint.o tai_unpack.o"
// dep: "tai_add.o: tai/tai_add.c tai.h libowfat/uint64.h"
// we will open *.h and */*.c, looking for #include directives

// a holds pointers to filenames in a line for libdep
size_t al;	// allocated and length for a
char* a[1024];	// array of pointers to strings for libdep

// buf holds the current line for libdep; used by objdup, flushlibdep
size_t bl;	// allocated and length for buf
char buf[8196];	// will assert if not enough, increase size then

// curlibbuf holds the name of the library, used by flushlibdep
const char* curlib;
char curlibbuf[256];

// libs holds an array of the libdep strings for each library
char* libs[64];	// will assert if not enough, increase size then
size_t nl;	// index of next free slot in libs

// For qsort
static int scmp(const void* a,const void* b) {
  return strcmp(*(char**)a,*(char**)b)>0;
}

// Are we still working on the same library? Used by addlibdep
// If not we need to flush the string we have been building and make a new one
static bool samelib(const char* s) {
  return s == curlib;
}

// Add current libcur_dep plus \n to libdep, and reset libdep_cur
static void flushlibdep(const char* libname) {
  if (curlibbuf[0]) {	// we were working on a library
    size_t i;
    // curlibbuf is "byte", make it "BYTE" instead
    for (i=0; curlibbuf[i]>='a' && curlibbuf[i]<='z'; ++i)
      curlibbuf[i]-=32;

    // sort all the file names alphabetically
    qsort(a,al,sizeof(a[0]),scmp);

    // count bytes needed for all file names
    size_t n=0;
    // don't worry about integer overflows since all fit into global buf
    for (i=0; i<al; ++i)
      n+=strlen(a[i])+1;

    // add 50 bytes scratch space
    char s[n+50];

    // s="BYTE"
    char* c=stpcpy(s,curlibbuf);
    // s="BYTE_OBJS="
    c=stpcpy(c,"_OBJS=");
    // add sorted file names
    for (i=0; i<al; ++i) {
      // we allocated enough for all the file names + 50
      c=stpcpy(c,a[i]);
      *c++=' ';
    }
    c[-1]='\n';
    assert(nl<sizeof(libs)/sizeof(libs[0]));
    libs[nl++]=strndup(s,c-s);
  }
  memset(curlibbuf,0,sizeof(curlibbuf));
  if (libname) {
    assert(strlen(libname)<sizeof(curlibbuf)-1);
    strcpy(curlibbuf,libname);
    curlib=libname;
  }
}

const char* objext="o";
static size_t extlen;

// add filename to dependency list for current library (for libdep)
// called with foo.c from readdir, will add foo.o to dependencies string
static char* objdup(const char* s,size_t len) {
  char* r;
  r=memchr(s,'.',len);	// we get called with *.c from a subdir
  assert(r);		// so they must contain a '.' (expect exactly one)
  ++r;			// point to 1st char past '.'
  size_t ofs=r-s;
  if (extlen==0) extlen=strlen(objext);	// initialize
  size_t needed=ofs+extlen+1;
  assert(len<256 && needed<sizeof(buf)-bl);
  r=memcpy(buf+bl,s,ofs);
  memcpy(buf+bl+ofs,objext,extlen+1);	// +1 to include 0 terminator
  bl+=needed;
  return r;
}

enum { MAPLEN=1000 };
const char* map[MAPLEN];
size_t mapuse;
const char* headermap[256];
size_t headermapuse;

// Called when we parsed a whole file for #include directives and built
// the dependency string. Adds it to a map so we can in the end do
// transitive dependencies without having to open and parse header files
// multiple times.
static void adddepstolist(const char* s) {
  // we a regular list for all files and a separate list for header
  // files, because *.c files don't have other *.c files are dependency.
  // we'll only need to search for *.h
  const char* x=strchr(s,':');
  if (x && x-s>3 && x[-2]=='.' && x[-1]=='h') {
    // bounds check
    if (headermapuse>=sizeof(headermap)/sizeof(headermap[0])) {
      write(2,"increase size of headermap in n.c!\n",35);
      exit(1);
    }
    // add entry
    headermap[headermapuse++]=strdup(s);
  } else {
    // make sure we don't exceed MAPLEN
    if (mapuse>=MAPLEN) {
      write(2,"increase MAPLEN in n.c!\n",24);
      exit(1);
    }
    // add entry to list
    map[mapuse++]=strdup(s);
  }
}

// Look for filename s in the map of dependencies
// Return pointer to map entry or NULL
static const char* finddep(const char* s,size_t l,const char** ret) {
  size_t i;
  const char** m=map;
  size_t ml=mapuse;
  if (l>3 && s[l-2]=='.' && s[l-1]=='h') {
    // we put the headers in a separate list so we don't have to work so
    // much here
    m=headermap;
    ml=headermapuse;
    // if we are looking for "libowfat/byte.h", redirect to "byte.h"
    if (l>10 && !memcmp(s,"libowfat/",9)) {
      s+=9;
      l-=9;
    }
  }
  // the strings in the map look like this:
  // "byte_copy.o: byte/byte_copy.c byte.h libowfat/compiler.h"
  // so to find a file, we look for ':' as a separator
  for (i=0; i<ml; ++i) {
    const char* b=strchr(m[i],':');
    if (!b)
      return 0;	// can't happen
    if (l == (size_t)(b-m[i]) && !memcmp(s,m[i],l)) {
      ++b;
      if (*b==' ') ++b;
      if (ret) *ret=m[i];
      return b;
    }
  }
  return 0;
}

// a self-growing bag of strings with deduplication
struct stringbag {
  char* base;
  size_t l,a;
};

// init
void init_stringbag(struct stringbag* sb) {
  sb->base=malloc(4096);
  if (sb->base==0) {
    perror("malloc");
    exit(111);
  }
  sb->l=0;
  sb->a=4096;
}

// add s to stringbag (returns offset relative to sb->base)
static size_t addstring(struct stringbag* sb,const char* s,size_t l) {
  assert(l<=127);
  if (!sb->base) init_stringbag(sb);
  size_t i;
  for (i=0; i<sb->l; i+=sb->base[i]+1) {
    if (sb->base[i] == (int)l && !memcmp(sb->base+i+1,s,l)) {
      // found!
      return i;
    }
  }
  if (sb->l+l+2 > sb->a) {
    sb->a += 4096;
    char* x = realloc(sb->base, sb->a);
    if (!x) {
      perror("realloc");
      exit(111);
    }
    sb->base = x;
  }
  sb->base[i]=l;
  memcpy(sb->base+i+1, s, l+1);
  sb->l = i+1+l;
  return i;
}

// reset stringbag to empty for reuse
static void resetbag(struct stringbag* sb) {
  sb->l=0;
}

// taken from str/str_chr.c
size_t str_chr(const char *in, char needle) {
  size_t i;
  for (i=0; in[i] && in[i]!=needle; ++i) ;
  return i;
}

static void followdep(struct stringbag* sb, const char* s) {
  // "uint64_pack.o: uint/uint64_pack.c uint64.h uint32.h"

  const char* c=strchr(s,':');
  if (!c || c[1]!=' ') return;	// either not found or no dependencies
  c+=2;		// c = "uint/uint64_pack.c uint64.h uint32.h"
  while (*c && *c!=' ') {
    size_t n=str_chr(c,' ');
    size_t prev=sb->l;
    size_t ofs=addstring(sb,c,n);
    if (ofs>=prev) {	// was not the bag, need to recurse into it
      const char* x;
      if (finddep(c,n,&x))
	followdep(sb,x);
    }
    c += n + (c[n]==' ');
  }

}

static void dumpdep(const char* s) {
  static struct stringbag sb;
  if (!sb.base) init_stringbag(&sb);

  followdep(&sb,s);

  size_t i;
  i=str_chr(s,' ');
  printf("%.*s",(int)i,s);
  for (i=0; i<sb.l; i+=sb.base[i]+1) {
    printf(" %.*s",sb.base[i],sb.base+i+1);
  }
  putchar('\n');

  resetbag(&sb);
}

// dump all dependencies
static void dumpdeps() {
  size_t i;
  for (i=0; i<mapuse; ++i) {
    dumpdep(map[i]);
//    puts(map[i]);
  }
}

// pn == "byte/byte_copy.c"
// fn == "byte_copy.c"
// s == pointer to memory mapped file contents
// l == length of memory map
static void find_deps(const char* pn,const char* fn,const char* s,size_t l) {
  // go through memory mapped file, look for #include
  char d[1024];
  // we want an output like "byte_copy.o: byte/byte_copy.c byte.h libowfat/compiler.h"
  memset(d,0,sizeof d);
  assert(strlen(fn)<256);	// more like 10 in practice

  char* c=stpcpy(d,fn);
  assert(c>d);	// empty filename!?

  int header=0;

  // filename ends with .c, but we want .o
  if (c-d>2 && !strcmp(c-2,".h")) {
    // it's a header file, we don't need to change .c to .o
    header=1;
  } else {
    --c;
    assert(c+extlen+5 < d+sizeof(d));
    c=stpcpy(c,objext);
  }
  c=stpcpy(c,": ");

  if (header) {
    --c;	// prevent "  "
  } else {
    assert(c+strlen(pn)+5 < d+sizeof(d));
    c=stpcpy(c,pn);
  }

  while (l) {
    if (*s == '#') {
      if (l>sizeof("#ifdef UNITTEST") && !memcmp(s,"#ifdef UNITTEST",15)) {
	// we only care until #ifdef UNITTEST
	break;
      }
      if (l>9 && !memcmp(s,"#include ",9)) {
      // I never put spaces between # and ifdef
	if (s[9]=='"') {
	  const char* f=s+10;
	  size_t i;
	  for (i=0; f+i<s+l && f[i]!='"'; ++i) ;

	  assert(c+i+5 < d+sizeof(d));
	  *c=' ';
	  c=memcpy(c+1,f,i)+i;
	} else if (s[9]=='<') {
	  const char* f=s+10;
	  size_t i;
	  for (i=0; f+i<s+l && f[i]!='>'; ++i) ;
	  // we only care about <libowfat/...>
	  if (i>9 && !memcmp(f,"libowfat/",9)) {
	    assert(c+i+5 < d+sizeof(d));
	    *c=' ';
	    c=memcpy(c+1,f,i)+i;
	  }
	}
      }
    }
    const char* n=memchr(s,'\n',l);
    if (!n) break;	// this was the last line
    ++n;
    l-=(n-s);
    s=n;
  }
  *c=0;
  adddepstolist(d);
//  *c='\n';
//  write(1,d,c-d+1);
}

// fn is byte/byte_copy.c, f is byte_copy.c (both can be the same)
static void adddeps(const char* fn, const char* f) {
  size_t l;
  const char* x=mmap_read(fn,&l);
  if (x) {
    find_deps(fn,f,x,l);
    mmap_unmap(x,l);
  }
}

static void addlibdep(const char* p,size_t plen,const char* f) {
  // if p is "byte", libdep_cur should start with BYTE_OBJS
  // if it does, append f to libdep_cur
  // if it doesn't, append libdep_cur to libdep, set up libdep_cur
  size_t flen;
  if (plen>255 || (flen=strlen(f))>255) return;
  if (samelib(p)) {
    a[al++]=objdup(f,flen);
  } else {
    flushlibdep(p);
    a[0]=objdup(f,flen);
    al=1;
  }

  adddeps(fmt_strm_alloca(p,"/",f),f);
}

int main() {
  DIR* D=opendir(".");
  assert(D);
  struct dirent* d;
  while ((d=readdir(D))) {
    // we are interested in *.h and */*.c
    if (d->d_type==DT_REG) {	// *.h
      const char* dot=strrchr(d->d_name,'.');
      if (dot && !strcmp(dot,".h")) {
	adddeps(d->d_name,d->d_name);
      }
    } else if (d->d_name[0]!='.' && d->d_type==DT_DIR &&
	strcmp(d->d_name,"test") &&
	strcmp(d->d_name,"examples")) {
      size_t n=strlen(d->d_name);
      DIR* DS=opendir(d->d_name);
      if (DS) {
	struct dirent* ds;
	while ((ds=readdir(DS))) {
	  const char* dot=strrchr(ds->d_name,'.');
	  if (dot && !strcmp(dot,".c")) {
	    addlibdep(d->d_name,n,ds->d_name);
//	    printf("%s/%s\n",d->d_name,ds->d_name);
	  }
	}
	closedir(DS);
      }
    }
  }
  closedir(D);
  flushlibdep(NULL);

  // libs now as a list of lines for the file
  // sort them alphabetically
  qsort(libs,nl,sizeof(libs[0]),scmp);
  // save string lengths in ll
  size_t ll[sizeof(libs)/sizeof(libs[0])];
  size_t i,n;
  // count total bytes
  for (i=n=0; i<nl; ++i)
    n+=(ll[i]=strlen(libs[i]));
  if (!n) {
    fprintf(stderr,"no files?!\n");
    return 111;
  }
  // copy all library strings in one buffer so we can compare against
  // existing libdep file
  char* x=malloc(n);
  char* y=x;
  for (i=0; i<nl; ++i) {
    memcpy(y,libs[i],ll[i]);
    y+=ll[i];
  }

  // overwrite libdep with new data if changed
  size_t libdeplen=0;
  const char* libdep=mmap_read("libdep",&libdeplen);
  if (libdeplen != n || memcmp(libdep,x,n)) {	// different contents
    int fd=open_trunc("libdep");
    assert(fd!=-1);
    write(fd,x,n);
    close(fd);
    fprintf(stderr,"libdep updated\n");
  } else
    fprintf(stderr,"libdep is unchanged\n");
  free(x); y=0;

#if 0
  const char* z;
  if (finddep("libowfat/buffer.h",17,&z))
    dumpdep(z);
#endif
  dumpdeps();

//  write(1,libdep.s,libdep.len);
}
