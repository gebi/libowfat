#include <unistd.h>
#include "textcode.h"
#include "str.h"
#include "buffer.h"

int main(int argc,char* argv[]) {
  char buf[4096];
  buffer filein;
  int fd=1;
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
  /* skip to "^begin " */
  for (;;) {
    char line[1000];	/* uuencoded lines can never be longer than 64 characters */
    int l;
    if ((l=buffer_getline(&filein,line,(sizeof line)-1))==0) {
      buffer_putsflush(buffer_2,"warning: hit end of file without finding any uuencoded data!\n");
      return 0;
    }
    if (l>0 && buf[l-1]=='\r') --l;	/* kill DOS line endings */
    buf[l]=0;
    if (!str_diffn(line,"begin ",6)) break;
  }
  /* read uuencoded lines */
  for (;;) {
    char line[1000];	/* uuencoded lines can never be longer than 64 characters */
    int l;
    if ((l=buffer_getline(&filein,line,(sizeof line)-1))==0) {
      buffer_putsflush(buffer_2,"warning: hit end of file without finding \"end\"!\n");
      return 0;
    }
    if (l>0 && buf[l-1]=='\r') --l;	/* kill DOS line endings */
    buf[l]=0;
    if (!str_diffn(line,"begin ",6)) break;
  }
  /* TODO */
}
