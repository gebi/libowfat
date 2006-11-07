#include "stralloc.h"
#include "buffer.h"

static ssize_t dummyreadwrite(int fd,char* buf,size_t len) {
  (void)fd;
  (void)buf;
  (void)len;
  return 0;
}

void buffer_fromsa(buffer* b,stralloc* sa) {
  b->x=sa->s;
  b->p=0;
  b->n=sa->len;
  b->a=sa->a;
  b->fd=0;
  b->op=dummyreadwrite;
}
