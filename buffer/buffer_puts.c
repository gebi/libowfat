#include "str.h"
#include "buffer.h"

int buffer_puts(buffer* b,const char* x) {
  buffer_put(b,x,str_len(x));
}
