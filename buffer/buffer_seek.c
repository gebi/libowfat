#include "buffer.h"

void buffer_seek(buffer* b,unsigned long int len) {
  unsigned long int n=b->p+len;
  if (n<b->p) n=b->p;
  if (n>b->n) n=b->n;
  b->p=n;
}
