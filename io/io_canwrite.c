#include <unistd.h>
#include <sys/time.h>
#include <poll.h>
#include <errno.h>
#include "io_internal.h"

int64 io_canwrite() {
  io_entry* e;
  if (first_writeable==-1) return -1;
  e=array_get(&io_fds,sizeof(io_entry),first_writeable);
  if (e && e->canwrite) {
    int64 r=first_writeable;
    first_writeable=e->next_write;
    return r;
  }
  return -1;
}
