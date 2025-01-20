#include <libowfat/buffer.h>
#include <libowfat/fmt.h>

int buffer_putxlong(buffer *b,unsigned long l) {
  char buf[FMT_XLONG];
  return buffer_put(b,buf,fmt_xlong(buf,l));
}

