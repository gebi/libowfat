#include <unistd.h>
#include "io_internal.h"

void io_close(int64 d) {
  io_entry* e;
  if ((e=array_get(&io_fds,sizeof(io_entry),d))) {
    e->inuse=0;
    io_dontwantread(d);
    io_dontwantwrite(d);
  }
  close(d);
}
