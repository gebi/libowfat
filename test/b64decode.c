#include <string.h>
#include "buffer.h"
#include "textcode.h"
#include "havealloca.h"

void b64encode(const char* c) {
  char* buf=alloca(strlen(c)*2+4);
  unsigned int dlen;
  scan_base64(c,buf,&dlen);
  buffer_put(buffer_1,buf,dlen);
}

main(int argc,char* argv[]) {
  int i;
  for (i=1; i<argc; ++i) {
    b64encode(argv[i]);
    buffer_flush(buffer_1);
  }
  if (argc<2) {
    char src[1024];
    int len;
    while ((len=read(0,src,sizeof(src)-1))>0) {
      src[len]=0;
      b64encode(src);
    }
    buffer_putnlflush(buffer_1);
    if (len==-1) return(1);
  }
  return 0;
}
