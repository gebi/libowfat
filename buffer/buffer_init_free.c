#include "buffer.h"

void buffer_init_free(buffer* b,int (*op)(),int fd,
		 unsigned char* y,unsigned long int ylen) {
  buffer_init(b,op,fd,y,ylen);
  b->todo=FREE;
}
