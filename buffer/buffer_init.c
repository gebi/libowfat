#include "buffer.h"

void buffer_init(buffer* b,int (*op)(),int fd,char* y,unsigned int ylen) {
  b->op=op;
  b->fd=fd;
  b->x=y;
  b->n=ylen;
  b->p=0;
}
