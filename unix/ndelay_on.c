#include <sys/types.h>
#include <fcntl.h>

#include "ndelay.h"

#ifndef O_NDELAY
#define O_NDELAY O_NONBLOCK
#endif

int ndelay_on(int fd) {
  return fcntl(fd,F_SETFL,fcntl(fd,F_GETFL,0) | O_NDELAY);
}
