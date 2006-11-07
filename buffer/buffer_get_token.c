#include "byte.h"
#include "buffer.h"
#include "scan.h"

ssize_t buffer_get_token(buffer* b,char* x,size_t len,const char* charset,size_t setlen) {
  size_t blen;

  for (blen=0;blen<len;++blen) {
    register int r;
    if ((r=buffer_getc(b,x))<0) return r;
    if (r==0) { *x=0; break; }
    if (byte_chr(charset,setlen,*x)<setlen) break;
    ++x;
  }
  return blen;
}
