#include "buffer.h"

void buffer_seek(buffer* b,unsigned int len) {
  unsigned int n=b->p+len;
  if (n>b->n) n=b->n;
  b->p=n;
}
