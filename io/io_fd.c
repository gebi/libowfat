#include <sys/types.h>
#include <fcntl.h>

#include "io_internal.h"

/* put d on internal data structure, return 1 on success, 0 on error */
int io_fd(int64 d) {
  long r;
  io_entry* e;
  if ((r=fcntl(d,F_GETFL,0) & O_NDELAY) == -1)
    return 0;	/* file descriptor not open */
  if (!(e=array_allocate(&io_fds,sizeof(io_entry),d))) return 0;
  e->inuse=1;
  if (r&O_NDELAY) e->nonblock=1;
  return 1;
}
