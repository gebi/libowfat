#include "io_internal.h"

void io_timeout(int64 d,tai6464 t) {
  io_entry* e=array_get(&io_fds,sizeof(io_entry),d);
  if (e)
    e->timeout=t;
}
