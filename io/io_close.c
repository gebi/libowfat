#include <unistd.h>
#include <sys/types.h>
#ifdef __MINGW32__
#include <windows.h>
#else
#include <sys/mman.h>
#endif
#include "io_internal.h"

void io_close(int64 d) {
  io_entry* e;
  if ((e=array_get(&io_fds,sizeof(io_entry),d))) {
    e->inuse=0;
    e->cookie=0;
    io_dontwantread(d);
    io_dontwantwrite(d);
    if (e->mmapped) {
#ifdef __MINGW32__
      UnmapViewOfFile(e->mmapped);
      CloseHandle(e->mh);
#else
      munmap(e->mmapped,e->maplen);
#endif
      e->mmapped=0;
    }
  }
  close(d);
}
