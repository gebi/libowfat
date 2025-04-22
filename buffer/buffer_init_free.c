#include <libowfat/buffer.h>

void buffer_init_free(buffer* b,ssize_t (*op)(int fd,const void* buf,size_t l),int fd,
		 char* y,size_t ylen) {
  buffer_init(b,op,fd,y,ylen);
  b->deinit=buffer_free;
}

void buffer_init_free_forread(buffer* b,ssize_t (*op)(int fd,void* buf,size_t l),int fd,
		 char* y,size_t ylen) {
  buffer_init_forread(b,op,fd,y,ylen);
  b->deinit=buffer_free;
}
