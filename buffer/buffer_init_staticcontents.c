#include <libowfat/buffer.h>
#include <errno.h>

static ssize_t op(int fd,const void* buf,size_t l) {
  (void)fd;
  (void)buf;
  (void)l;
  errno=ENOENT;
  return 0;
}

void buffer_init_staticcontents(buffer* b, char* y, size_t len) {
  b->x=y;
  b->p=0; b->a=len; b->n=0;
  b->fd=-1;
  b->op.wop=op;
  b->deinit=0;
}

void buffer_init_staticcontents_forread(buffer* b, const char* y, size_t len) {
  b->x=(char*)y;
  b->p=0;
  b->a=b->n=len;
  b->fd=-1;
  b->op.wop=op;
  b->deinit=0;
}

#ifdef UNITTEST
#include <assert.h>

#undef UNITTEST
#include "byte/byte_copy.c"
#include "buffer/buffer_get.c"
#include "buffer/buffer_put.c"
#include "buffer/buffer_stubborn.c"
#include "buffer/buffer_stubborn2.c"
#include "buffer/buffer_flush.c"
#include "buffer/buffer_feed.c"

int main() {
  buffer b;
  buffer_init_staticcontents_forread(&b, "fnord", 5);
  char tmp[6];
  assert(buffer_get(&b, tmp, 6) == 5);
  assert(!memcmp(tmp,"fnord",5));
  buffer_init_staticcontents(&b, tmp, sizeof tmp);
  buffer_puts(&b, "foo\n");
  assert(!memcmp(tmp, "foo\n", 4));
}
#endif
