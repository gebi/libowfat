#include <unistd.h>
#include "io_internal.h"

void io_close(int64 d) {
  close(d);
  io_entry* e=array_get(&io_fds,sizeof(io_entry),d);
  if (e) e->inuse=0;
}
