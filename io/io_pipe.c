#include <unistd.h>
#include "io_internal.h"

int io_pipe(int64* d) {
  int fds[2];
  if (pipe(fds)==-1)
    return 0;
  if (io_fd(fds[1])) {
    if (io_fd(fds[0])) {
      d[0]=fds[0];
      d[1]=fds[1];
      return 1;
    }
    io_close(fds[1]);
  }
  close(fds[0]);
  return 0;
}
