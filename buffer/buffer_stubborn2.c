#include <errno.h>
#include "buffer.h"

int buffer_stubborn_read(ssize_t (*op)(),int fd,const char* buf, size_t len) {
  int w;
  for (;;) {
    if ((w=op(fd,buf,len))<0)
      if (errno == EINTR) continue;
    break;
  }
  return w;
}

