#include "byte.h"
#include "buffer.h"

int buffer_getn(buffer* b,char* x,unsigned int len) {
  int blen;

  for(blen=0;blen<len;++blen) {
    register int r;
    if ((r=buffer_getc(b,x))<0) return r;
    if (r==0) break;
    ++x;
  }
  return blen;
}
