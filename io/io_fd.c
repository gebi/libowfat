#include <sys/types.h>
#include <fcntl.h>

#include "io_internal.h"
#ifdef HAVE_KQUEUE
#include <sys/event.h>
#endif
#ifdef HAVE_EPOLL
#include <sys/epoll.h>
#endif

/* put d on internal data structure, return 1 on success, 0 on error */
int io_fd(int64 d) {
  long r;
  io_entry* e;
  if ((r=fcntl(d,F_GETFL,0) & O_NDELAY) == -1)
    return 0;	/* file descriptor not open */
  if (!(e=array_allocate(&io_fds,sizeof(io_entry),d))) return 0;
  e->inuse=1;
  if (r&O_NDELAY) e->nonblock=1;
  e->next_read=e->next_write=-1;
  if (io_waitmode==UNDECIDED) {
#if defined(HAVE_EPOLL)
    io_master=epoll_create(1000);
    if (io_master!=-1) io_waitmode=EPOLL;
#endif
#if defined(HAVE_KQUEUE)
    if (io_waitmode==POLL) {	/* who knows, maybe someone supports both one day */
      io_master=kqueue();
      if (io_master!=-1) io_waitmode=KQUEUE;
    }
#endif
  }
  return 1;
}
