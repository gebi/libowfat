#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "io_internal.h"

void io_nonblock(int64 d) {
  io_entry* e=array_get(&io_fds,sizeof(io_entry),d);
  fcntl(d,F_SETFL,fcntl(d,F_GETFL,0) | O_NDELAY);
  if (e) e->nonblock=1;
}
