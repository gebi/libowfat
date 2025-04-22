#include <errno.h>
#include "buffer.h"

ssize_t buffer_stubborn_read(ssize_t (*op)(int fd,char* buf,size_t len,void* cookie),int fd,char* buf, size_t len,void* cookie) {
  ssize_t w;
  for (;;) {
    if ((w=op(fd,buf,len,cookie))<0)
      if (errno == EINTR) continue;
    break;
  }
  return w;
}

