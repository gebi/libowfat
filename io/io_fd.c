#define _GNU_SOURCE
#define extern
#include "io_internal.h"
#undef extern
#ifdef HAVE_SIGIO
#include <sys/poll.h>
#include <signal.h>
#include <fcntl.h>
#endif
#include <sys/types.h>
#include <fcntl.h>

#ifdef HAVE_KQUEUE
#include <sys/event.h>
#endif
#ifdef HAVE_EPOLL
#include <inttypes.h>
#include <sys/epoll.h>
#endif
#include <unistd.h>

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
    first_readable=first_writeable=-1;
#if defined(HAVE_EPOLL)
    io_master=epoll_create(1000);
    if (io_master!=-1) io_waitmode=EPOLL;
#endif
#if defined(HAVE_KQUEUE)
    if (io_waitmode==UNDECIDED) {	/* who knows, maybe someone supports both one day */
      io_master=kqueue();
      if (io_master!=-1) io_waitmode=KQUEUE;
    }
#endif
#if defined(HAVE_SIGIO)
    alt_firstread=alt_firstwrite=-1;
    if (io_waitmode==UNDECIDED) {
      io_signum=SIGRTMIN+1;
      if (sigemptyset(&io_ss)==0 &&
          sigaddset(&io_ss,io_signum)==0 &&
          sigaddset(&io_ss,SIGIO)==0 &&
          sigprocmask(SIG_BLOCK,&io_ss,0)==0)
	io_waitmode=_SIGIO;
    }
#endif
  }
#if defined(HAVE_SIGIO)
  if (io_waitmode==_SIGIO) {
    fcntl(d,F_SETOWN,getpid());
    fcntl(d,F_SETSIG,io_signum);
#if defined(O_ONESIGFD) && defined(F_SETAUXFL)
    fcntl(d, F_SETAUXFL, O_ONESIGFD);
#endif
    fcntl(d,F_SETFL,fcntl(d,F_GETFL)|O_NONBLOCK|O_ASYNC);
  }
#endif
  return 1;
}
