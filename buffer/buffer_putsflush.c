#include "str.h"
#include "buffer.h"

int buffer_putsflush(buffer* b,const unsigned char* x) {
  return buffer_putflush(b,x,str_len(x));
}
