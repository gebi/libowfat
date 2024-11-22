#include <errno.h>
#include "array.h"
#include "buffer.h"

static ssize_t fail() {
  errno=EINVAL;
  return -1;
}

void buffer_fromarray(buffer* b,array* a) {
  if (array_failed(a)) {
    memset(b,0,sizeof *b);
    b->op=fail;
  } else
    buffer_frombuf(b,array_start(a),a->initialized);
}
