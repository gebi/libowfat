#include <string.h>
#include "buffer.h"
#include "textcode.h"

void urlencode(const char* c) {
  char* buf=alloca(strlen(c)*3+1);
  buffer_put(buffer_1,buf,fmt_urlencoded(buf,c,strlen(c)));
  buffer_putnlflush(buffer_1);
}

main(int argc,char* argv[]) {
  int i;
  for (i=1; i<argc; ++i) {
    urlencode(argv[i]);
  }
  return 0;
}
