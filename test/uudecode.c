#include <unistd.h>
#include "textcode.h"
#include "str.h"
#include "buffer.h"
#include "open.h"

int main(int argc,char* argv[]) {
  char buf[4096];
  char obuf[4096];
  buffer filein;
  buffer fileout;
  int fd=0;
  int ofd=-1;
  int found=0;
  char line[1000];	/* uuencoded lines can never be longer than 64 characters */
  int l;
  enum { BEFOREBEGIN, AFTERBEGIN, SKIPHEADER } state=BEFOREBEGIN;
  unsigned long mode=0,lineno=0;
  if (argc>1) {
    fd=open_read(argv[1]);
    if (fd<0) {
      buffer_puts(buffer_2,"error: could not open \"");
      buffer_puts(buffer_2,argv[1]);
      buffer_putsflush(buffer_2,"\"\n");
      return 1;
    }
  }
  buffer_init(&filein,read,fd,buf,sizeof buf);
again:
  /* skip to "^begin " */
  for (;;) {
    if ((l=buffer_getline(&filein,line,(sizeof line)-1))==0 && line[l]!='\n') {
      if (state!=BEFOREBEGIN) {
	buffer_puts(buffer_1,"premature end of file in line ");
	buffer_putulong(buffer_1,lineno);
	buffer_putsflush(buffer_1,"!\n");
	if (ofd>=0) {
	  buffer_flush(&fileout);
	  fchmod(ofd,mode);
	  close(ofd);
	}
	++found;
      }
      if (!found)
	buffer_putsflush(buffer_2,"warning: hit end of file without finding any uuencoded data!\n");
      return 0;
    }
    ++lineno;
    if (l>0 && line[l-1]=='\r') --l;	/* kill DOS line endings */
    line[l]=0;
    if (str_start(line,"begin ")) {
      if (state!=BEFOREBEGIN) {
	buffer_puts(buffer_1,"new begin without previous end in line ");
	buffer_putulong(buffer_1,lineno);
	buffer_putsflush(buffer_1,"!\n");
	if (ofd>=0) {
	  buffer_flush(&fileout);
	  fchmod(ofd,mode);
	  close(ofd);
	}
	++found;
      }
      state=BEFOREBEGIN;
      if (line[l=6+scan_8long(line+6,&mode)]==' ' && mode) {
	int i;
	++l;
	if (line[l]=='"') {
	  int m;
	  ++l;
	  line[str_chr(line+l,'"')]=0;
	}
	if (line[l+(i=str_rchr(line+l,'/'))]) l+=i+1;
	while (line[l]=='.') ++l;
	if (line[l]) {
	  ofd=open_excl(line+l);
	  if (ofd<0) {
	    buffer_puts(buffer_2,"error: could not create file \"");
	    buffer_puts(buffer_2,line+l);
	    buffer_putsflush(buffer_2,"\" (must not exist yet)\n");
	  } else {
	    buffer_puts(buffer_2,"decoding file \"");
	    buffer_puts(buffer_2,line+l);
	    buffer_putsflush(buffer_2,"\"\n");
	    state=AFTERBEGIN;
	    buffer_init(&fileout,write,ofd,obuf,sizeof obuf);
	    continue;
	  }
	}
      }
    } else if (str_equal(line,"end")) {
      if (ofd>=0) {
	buffer_flush(&fileout);
	fchmod(ofd,mode);
	close(ofd);
	ofd=-1;
      }
      ++found;
      state=BEFOREBEGIN;
      continue;
    } else if (!line[0]) {
      if (state==AFTERBEGIN)
	state=SKIPHEADER;
    } else {
      unsigned int scanned,x;
      char tmp[100];
      x=scan_uuencoded(line,tmp,&scanned);
      if (!x) {
	if (state==AFTERBEGIN) {
	  buffer_puts(buffer_1,"parse error in line ");
	  buffer_putulong(buffer_1,lineno);
	  buffer_puts(buffer_1,": \"");
	  buffer_puts(buffer_1,line);
	  buffer_putsflush(buffer_1,"\"\n");
	  return 1;
	}
      } else {
	if (ofd>=0)
	  buffer_put(&fileout,tmp,scanned);
      }
    }
  }
}
