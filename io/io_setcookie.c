
#include <unistd.h>
#include "io_internal.h"

void io_setcookie(int64 d,void* cookie) {
  io_entry* e;
  if ((e=array_get(&io_fds,sizeof(io_entry),d)))
    e->cookie=cookie;
}
