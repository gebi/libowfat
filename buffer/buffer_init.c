#include <libowfat/buffer.h>

void buffer_init(buffer* b,ssize_t (*op)(int fd,const void* buf,size_t l),int fd,
		 char* y,size_t ylen) {
  b->op.wop=op;
  b->fd=fd;
  b->x=y;
  b->a=ylen;
  b->p=0;
  b->n=0;
  b->cookie=0;
  b->deinit=0;
}

void buffer_init_forread(buffer* b,ssize_t (*op)(int fd,void* buf,size_t l),int fd,
		 char* y,size_t ylen) {
  buffer_init(b,(void*)op,fd,y,ylen);
}
