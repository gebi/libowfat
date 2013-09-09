#include "stralloc.h"
#include "buffer.h"

void buffer_fromsa(buffer* b,stralloc* sa) {
  buffer_frombuf(b,sa->s,sa->len);
}
