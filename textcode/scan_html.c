#include <inttypes.h>
#include <string.h>
#include "entities.h"

#include "fmt.h"
#include "textcode.h"
#include "haveinline.h"
#include "scan.h"
#include "case.h"
#include "str.h"

static const char* lookup(size_t ofs,const char* t) {
  if (ofs>entities.tab[0]) return 0;
  while (ofs<entities.tab[0]) {
    unsigned char ch=entities.tab[ofs]&0xff;
    if (ch==(unsigned char)*t || (!ch && *t==';')) {
      if (!ch || *t==';')
	return entities.data+(entities.tab[ofs]>>8);
      else
	return lookup(entities.tab[ofs]>>8,t+1);
    } else
      ++ofs;
    if (!ch) break;
  }
  return NULL;
}

enum htmlmode {	/* <a href="http://example.com/&quot;foo">libowfat&lt;home</a> */
  OUTSIDE,	/*                                        ^^^^^^^^^^^^^^^^ -> `libowfat<home` */
  TAGARG,	/*         ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ -> `http://example.com/"foo´ */
};

static size_t scan_html_inner(const char *src,char *dest,size_t *destlen,enum htmlmode mode) {
  register const unsigned char* s=(const unsigned char*) src;
  size_t written=0,i;
  int dq=0;
  for (i=0; s[i]; ++i) {
    if (s[i]=='&') {
      const char* utf8;
      if (s[i+1]=='#') {
	unsigned long l;
	size_t j;
	if ((s[i+2]&~32)=='X') {
	  j=scan_xlong(src+i+3,&l);
	  if (!j) j+=3;
	} else {
	  j=scan_ulong(src+i+2,&l);
	  if (!j) j+=3;
	}
	if (s[i+j]==';') {
	  i+=j;
	  written+=fmt_utf8(dest+written,l);
	} else {
	  dest[written++]='&';
	}
	continue;
      }
      utf8=lookup(1,src+i+1);
      if (utf8) {
	size_t l=strlen(utf8);
	memcpy(dest+written,utf8,l);
	written+=l;
	i+=2+str_chr(src+i+2,';');
	continue;
      } else
	dest[written]='&';
    } else if (s[i]=='<') {
      if (mode == OUTSIDE) break;
      if (case_starts((const char*)s+i+1,"br>")) {
	dest[written]='\n';
	i+=3;
      } else if (case_starts((const char*)s+i+1,"p>")) {
	dest[written]='\n'; ++written;
	dest[written]='\n';
	i+=3;
      } else
	dest[written]=s[i];
    } else if (s[i]=='"' && mode==TAGARG) {
      if (i==0) { dq=1; continue; }
      break;
    } else if (mode==TAGARG && !dq && (s[i]==' ' || s[i]=='\t' || s[i]=='\n'))
      break;
    else
      dest[written]=s[i];
    ++written;
  }
  *destlen=written;
  return i;
}

size_t scan_html_tagarg(const char *src,char *dest,size_t *destlen) {
  return scan_html_inner(src,dest,destlen,TAGARG);
}

size_t scan_html(const char *src,char *dest,size_t *destlen) {
  return scan_html_inner(src,dest,destlen,OUTSIDE);
}

#ifdef UNITTEST
#include <assert.h>

int main() {
  char* html="<a href=\"http://example.com/&quot;foo\">libowfat&lt;home</a>";
  char buf[100];
  size_t destlen;
  assert(scan_html(html,buf,&destlen)==0 && destlen==0);
  assert(scan_html(strchr(html,'>')+1,buf,&destlen)==16 && destlen==13 && !memcmp(buf,"libowfat<home",13));
  assert(scan_html_tagarg(strchr(html,'"')+1,buf,&destlen)==28 && destlen==23 && !memcmp(buf,"http://example.com/\"foo",23));
}
#endif
