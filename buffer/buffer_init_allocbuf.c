#include <stdlib.h>
#include <unistd.h>
#include <libowfat/buffer.h>

int buffer_init_allocbuf(buffer* b, ssize_t (*op)(int fd,const void* buf,size_t l), int fd, size_t ylen) {
  if (fd==-1) return -1;
  char* thebuffer;
  if (!(thebuffer=malloc(ylen)))
    return -1;
  buffer_init_free(b, op, fd, thebuffer, ylen);
  return 0;
}

int buffer_init_allocbuf_forread(buffer* b, ssize_t (*op)(int fd,void* buf,size_t l), int fd, size_t ylen) {
  return buffer_init_allocbuf(b,(void*)op,fd,ylen);
}
