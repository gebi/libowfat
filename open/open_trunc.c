#define _FILE_OFFSET_BITS 64
#include <unistd.h>
#include <fcntl.h>
#ifndef INTERNAL
#include <libowfat/open.h>
#endif

#ifndef O_NDELAY
#define O_NDELAY 0
#endif

int open_trunc(const char *filename) {
  return open(filename,O_WRONLY|O_NDELAY|O_TRUNC|O_CREAT,0644);
}
