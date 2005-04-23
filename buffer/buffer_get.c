#include "byte.h"
#include "buffer.h"

int buffer_get(buffer* b,char* x,unsigned long int len) {
  int blen;
  if ((blen=buffer_feed(b))>=len)
    blen=len;
  if (blen<=0) return blen;
  byte_copy(x,blen,b->x+b->p);
  b->p+=blen;
  return blen;
}
