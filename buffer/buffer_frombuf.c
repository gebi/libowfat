#include "stralloc.h"
#include "buffer.h"

static ssize_t dummyreadwrite(int fd,void* buf,size_t len) {
  (void)fd;
  (void)buf;
  (void)len;
  return 0;
}

void buffer_frombuf(buffer* b,const char* x,size_t l) {
  b->x=(char*)x;
  b->p=0;
  b->n=l;
  b->a=l;
  b->fd=0;
  b->op.rop=dummyreadwrite;
  b->deinit=0;
}
