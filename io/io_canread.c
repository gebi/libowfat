#include <unistd.h>
#include <sys/time.h>
#include <poll.h>
#include <errno.h>
#include "io_internal.h"

int64 io_canread() {
  io_entry* e;
  if (first_readable==-1) return -1;
  e=array_get(&io_fds,sizeof(io_entry),first_readable);
  if (e && e->canread) {
    int64 r=first_readable;
    first_readable=e->next_read;
    return r;
  }
  return -1;
}
