#include <time.h>
#include <buffer.h>

main() {
  char buf[100];
  buffer_put(buffer_1,buf,fmt_httpdate(buf,time(0)));
  buffer_putnlflush(buffer_1);
}
