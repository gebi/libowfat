#include "byte.h"
#include "buffer.h"

int buffer_get(buffer* b,char* x,unsigned long int len) {
  unsigned long done;
  int blen;
  done=0;
  while (len) {
    if ((blen=buffer_feed(b))<=0) return blen;
    if ((unsigned long int) blen>=len)
      blen=len;
    byte_copy(x,blen,b->x+b->p);
    b->p+=blen;
    len-=blen;
    x+=blen;
    done+=blen;
  }
  return done;
}
