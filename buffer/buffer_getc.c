#include "byte.h"
#include "buffer.h"

int buffer_getc(buffer* b,char* x) {
  if (b->p==b->n) {
    register int blen;
    if ((blen=buffer_feed(b))<=0) return blen;
  }
  *x=b->x[b->p];
  ++b->p;
  return 1;
}
