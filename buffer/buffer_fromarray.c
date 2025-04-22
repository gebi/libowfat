#include <errno.h>
#include "array.h"
#include "buffer.h"

static ssize_t fail(int fd,void* buf,size_t l) {
  (void)fd;
  (void)buf;
  (void)l;
  errno=EINVAL;
  return -1;
}

void buffer_fromarray(buffer* b,array* a) {
  if (array_failed(a)) {
    memset(b,0,sizeof *b);
    b->op.rop=fail;
  } else
    buffer_frombuf(b,array_start(a),a->initialized);
}
