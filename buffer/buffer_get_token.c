#include "byte.h"
#include "buffer.h"
#include "scan.h"

int buffer_get_token(buffer* b,char* x,unsigned int len,const char* charset,unsigned int setlen) {
  int blen;

  for (blen=0;blen<len;++blen) {
    register int r;
    if ((r=buffer_getc(b,x))<0) return r;
    if (r==0) { *x=0; break; }
    if (byte_chr(charset,setlen,*x)<setlen) break;
    ++x;
  }
  return blen;
}
