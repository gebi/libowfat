#include "buffer.h"
#include "fmt.h"

int buffer_putlong(buffer *b,unsigned long l) {
  char buf[FMT_ULONG];
  return buffer_put(b,buf,fmt_long(buf,l));
}

