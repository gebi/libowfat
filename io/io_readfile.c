#define _FILE_OFFSET_BITS 64
#include <unistd.h>
#include <fcntl.h>
#include "io_internal.h"

int io_readfile(int64* d,const char* s) {
  long fd=open(s,O_RDONLY);
  if (fd != -1) {
    if (io_fd(fd)) {
      *d=fd;
      return 1;
    }
    close(fd);
  }
  return 0;
}
