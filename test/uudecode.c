#include <unistd.h>
#include <stdlib.h>
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
  int broken_encoder=0;
  int found=0;
  char line[1000];	/* uuencoded lines can never be longer than 64 characters */
  int l;
  enum { BEFOREBEGIN, AFTERBEGIN, SKIPHEADER } state=BEFOREBEGIN;
  enum { UUDECODE, YENC } mode=UUDECODE;
  unsigned long fmode=0,lineno=0;
  unsigned long offset,endoffset,totalsize,linelen; /* used only for yenc */

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
	  fchmod(ofd,fmode);
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
	  fchmod(ofd,fmode);
	  close(ofd);
	}
	++found;
      }
      state=BEFOREBEGIN;
      if (line[l=6+scan_8long(line+6,&fmode)]==' ' && fmode) {
	int i;
	++l;
	mode=UUDECODE;
foundfilename:
	if (line[l]=='"') {
	  int m;
	  ++l;
	  line[str_chr(line+l,'"')]=0;
	}
	if (line[l+(i=str_rchr(line+l,'/'))]) l+=i+1;
	while (line[l]=='.') ++l;
	if (line[l]) {
	  if (mode==YENC)
	    ofd=open_write(line+l);
	  else
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
	fchmod(ofd,fmode);
	close(ofd);
	ofd=-1;
      }
      ++found;
      state=BEFOREBEGIN;
      continue;
    } else if (str_start(line,"=ybegin ")) {
      char* filename=strstr(line," name=");
      if (!filename) {
invalidybegin:
	buffer_puts(buffer_2,"invalid =ybegin at line ");
	buffer_putulong(buffer_2,lineno);
	buffer_putsflush(buffer_2,".\n");
	continue;
      }
      l=filename-line+6;
      if (!(filename=strstr(line," size="))) goto invalidybegin;
      if (filename[6+scan_ulong(filename+6,&totalsize)] != ' ') goto invalidybegin;
      if (!(filename=strstr(line," line="))) goto invalidybegin;
      if (filename[6+scan_ulong(filename+6,&linelen)] != ' ') goto invalidybegin;
      mode=YENC;
      goto foundfilename;
    } else if (str_start(line,"=ypart ")) {
      char* tmp=strstr(line," begin=");
      char c;
      if (!tmp) {
invalidpart:
	buffer_puts(buffer_2,"invalid =ypart at line ");
	buffer_putulong(buffer_2,lineno);
	buffer_putsflush(buffer_2,".\n");
	continue;
      }
      c=tmp[7+scan_ulong(tmp+7,&offset)];
      if (c!=' ' && c!=0) goto invalidpart;
      if (!(tmp=strstr(line," end="))) goto invalidpart;
      c=tmp[5+scan_ulong(tmp+5,&endoffset)];
      if (c!=' ' && c!=0) goto invalidpart;
      --offset; endoffset;
      if (endoffset<offset || endoffset>totalsize) goto invalidpart;
      lseek(ofd,offset,SEEK_SET);
      continue;
    } else if (str_start(line,"=yend")) {
      unsigned long cur;
      if (ofd>=0) {
	off_t rlen;
	buffer_flush(&fileout);
	rlen=lseek(ofd,0,SEEK_CUR)-offset;
	if (rlen != endoffset-offset) {
	  int toomuch=rlen-(endoffset-offset);
	  buffer_puts(buffer_2,"warning: part size ");
	  buffer_putulong(buffer_2,rlen);
	  buffer_puts(buffer_2,", expected ");
	  buffer_putulong(buffer_2,endoffset-offset);
	  buffer_putsflush(buffer_2,"!\n");
	}
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
      char tmp[300];
      switch (mode) {
      case UUDECODE: x=scan_uuencoded(line,tmp,&scanned); break;
      case YENC:
	/* work around broken yenc encoders */
	if ((line[0]=='.' && line[1]=='.') ||
	    (line[0]=='>' && line[1]=='.')) {
	  if (l>linelen && line[l-2]!='=') {
	    if (!broken_encoder) {
	      broken_encoder=1;
	      buffer_putsflush(buffer_2,"compensating for broken encoder...\n");
	    }
	    x=scan_yenc(line+1,tmp,&scanned);
	    break;
	  }
	}
	x=scan_yenc(line,tmp,&scanned);
	break;
      }
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
