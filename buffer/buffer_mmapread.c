#include <libowfat/buffer.h>
#include <libowfat/mmap.h>

static ssize_t op(int fd,void* buf,size_t l) {
  (void)fd;
  (void)buf;
  (void)l;
  return 0;
}

int buffer_mmapread(buffer* b,const char* filename) {
  if (!(b->x=(char*)mmap_read(filename,&b->n))) return -1;
  b->p=0; b->a=b->n;
  b->fd=-1;
  b->op.rop=op;
  b->deinit=buffer_munmap;
  return 0;
}
