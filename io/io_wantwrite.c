#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "io_internal.h"

void io_wantwrite(int64 d) {
  io_entry* e=array_get(&io_fds,sizeof(io_entry),d);
  if (!e) return;
  if (!e->wantread && !e->wantwrite) ++io_wanted_fds;
  e->wantwrite=1;
}
