#include "buffer.h"

void buffer_init(buffer* b,int (*op)(),int fd,
		 char* y,unsigned long int ylen) {
  b->op=op;
  b->fd=fd;
  b->x=y;
  b->a=ylen;
  b->p=0;
  b->n=0;
  b->todo=NOTHING;
}
