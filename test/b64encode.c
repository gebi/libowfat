#include <unistd.h>
#include <string.h>
#include "buffer.h"
#include "textcode.h"
#include "havealloca.h"

void b64encode(const char* c) {
  char* buf=alloca(strlen(c)*2+4);
  buffer_put(buffer_1,buf,fmt_base64(buf,c,strlen(c)));
  if (isatty(1))
    buffer_putnlflush(buffer_1);
  else
    buffer_flush(buffer_1);
}

main(int argc,char* argv[]) {
  int i;
  for (i=1; i<argc; ++i) {
    b64encode(argv[i]);
  }
  if (argc<2) {
    char src[1024];
    int len;
    while ((len=read(0,src,sizeof(src)-1))>0) {
      if (len==-1) return(1);
      src[len]=0;
      b64encode(src);
    }
  }
  return 0;
}
