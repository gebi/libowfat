#include <unistd.h>
#include "io_internal.h"

void* io_getcookie(int64 d) {
  io_entry* e;
  e=array_get(&io_fds,sizeof(io_entry),d);
  return e?e->cookie:0;
}
