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
    char line[1000];	/* uuencoded lines can never be longer than 64 characters */
    int l;
    if ((l=buffer_getline(&filein,line,(sizeof line)-1))==0 && line[l]!='\n') {
      if (!found)
	buffer_putsflush(buffer_2,"warning: hit end of file without finding any uuencoded data!\n");
      return 0;
    }
    ++lineno;
    if (l>0 && line[l-1]=='\r') --l;	/* kill DOS line endings */
    line[l]=0;
    if (!str_diffn(line,"begin ",6)) {
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
	    break;
	  }
	}
      }
    }
  }
  buffer_init(&fileout,write,ofd,obuf,sizeof obuf);
  /* read uuencoded lines */
  for (;;) {
    char line[1000];	/* uuencoded lines can never be longer than 64 characters */
    unsigned int scanned,x;
    char tmp[100];
    int l;
    if ((l=buffer_getline(&filein,line,(sizeof line)-1))==0) {
      buffer_putsflush(buffer_2,"warning: hit end of file without finding \"end\"!\n");
      return 0;
    }
    ++lineno;
    if (l>0 && line[l-1]=='\r') --l;	/* kill DOS line endings */
    line[l]=0;
    x=scan_uuencoded(line,tmp,&scanned);
    if (!x) {
      if (str_equal(line,"end")) {
	buffer_flush(&fileout);
	fchmod(ofd,mode);
	close(ofd);
	++found;
	goto again;
      } else {
parseerror:
	  buffer_puts(buffer_1,"parse error in line ");
	  buffer_putulong(buffer_1,lineno);
	  buffer_puts(buffer_1,": \"");
	  buffer_puts(buffer_1,line);
	  buffer_putsflush(buffer_1,"\"\n");
	  exit(1);
      }
    }
    buffer_put(&fileout,tmp,scanned);
  }
}
