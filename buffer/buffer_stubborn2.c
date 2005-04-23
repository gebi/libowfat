#include <errno.h>
#include "buffer.h"

int buffer_stubborn_read(int (*op)(),int fd,const char* buf, unsigned long int len) {
  int w;
  for (;;) {
    if ((w=op(fd,buf,len))<0)
      if (errno == EINTR) continue;
    break;
  }
  return w;
}

