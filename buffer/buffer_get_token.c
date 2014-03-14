#include "byte.h"
#include "buffer.h"
#include "scan.h"

ssize_t buffer_get_token(buffer* b,char* x,size_t len,const char* charset,size_t setlen) {
  size_t blen;

  if ((ssize_t)len<0) len=(ssize_t)(((size_t)-1)>>1);
  for (blen=0;blen<len;++blen) {
    register ssize_t r;
    if ((r=buffer_getc(b,x))<0) return r;
    if (r==0) { *x=0; break; }
    if (byte_chr(charset,setlen,*x)<setlen) break;
    ++x;
  }
  return (ssize_t)blen;
}
