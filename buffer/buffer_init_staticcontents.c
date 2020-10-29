#include "buffer.h"
#include <mmap.h>

static ssize_t op() {
  return 0;
}

void buffer_init_staticcontents(buffer* b, char* y, size_t len) {
  b->x=y;
  b->p=0; b->a=b->n=len;
  b->fd=-1;
  b->op=op;
  b->deinit=0;
}

#ifdef UNITTEST
#include <assert.h>

int main() {
  buffer b;
  buffer_init_staticcontents(&b, "fnord", 5);
  char tmp[6];
  assert(buffer_get(&b, tmp, 6) == 5);
  assert(!memcmp(tmp,"fnord",5));
  buffer_init_staticcontents(&b, tmp, sizeof tmp);
  buffer_puts(&b, "foo\n");
  assert(!memcmp(tmp, "foo\n", 4));
}
#endif
