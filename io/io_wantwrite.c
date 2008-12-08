#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "io_internal.h"
#ifdef HAVE_KQUEUE
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#endif
#ifdef HAVE_EPOLL
#include <inttypes.h>
#include <sys/epoll.h>
#include <byte.h>
#endif
#ifdef HAVE_SIGIO
#include <sys/poll.h>
#endif
#ifdef HAVE_DEVPOLL
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/devpoll.h>
#endif

void io_wantwrite(int64 d) {
  int newfd;
  io_entry* e=array_get(&io_fds,sizeof(io_entry),d);
  if (!e || e->wantwrite) return;
  newfd=(!e->wantread && !e->wantwrite);
  io_wanted_fds+=newfd;
#ifdef HAVE_EPOLL
  if (io_waitmode==EPOLL) {
    struct epoll_event x;
    byte_zero(&x,sizeof(x));	// to shut up valgrind
    x.events=EPOLLOUT;
    if (e->wantread) x.events|=EPOLLIN;
    x.data.fd=d;
    epoll_ctl(io_master,newfd?EPOLL_CTL_ADD:EPOLL_CTL_MOD,d,&x);
  }
#endif
#ifdef HAVE_KQUEUE
  if (io_waitmode==KQUEUE) {
    struct kevent kev;
    struct timespec ts;
    EV_SET(&kev, d, EVFILT_WRITE, EV_ADD|EV_ENABLE, 0, 0, 0);
    ts.tv_sec=0; ts.tv_nsec=0;
    kevent(io_master,&kev,1,0,0,&ts);
  }
#endif
#ifdef HAVE_DEVPOLL
  if (io_waitmode==DEVPOLL) {
    struct pollfd x;
    x.fd=d;
    x.events=POLLOUT;
    if (e->wantread) x.events|=POLLIN;
    write(io_master,&x,sizeof(x));
  }
#endif
#ifdef HAVE_SIGIO
  if (io_waitmode==_SIGIO) {
    struct pollfd p;
    p.fd=d;
    p.events=POLLOUT;
    switch (poll(&p,1,0)) {
    case 1: e->canwrite=1; break;
    case 0: e->canwrite=0; break;
    case -1: return;
    }
    if (e->canwrite) {
      debug_printf(("io_wantwrite: enqueueing %lld in normal write queue before %ld\n",d,first_readable));
      e->next_write=first_writeable;
      first_writeable=d;
    }
  }
#endif
#ifdef __MINGW32__
  printf("e->wantwrite == %d\n",e->wantwrite);
  if (!e->wantwrite) {
    e->next_write=first_writeable;
    e->canwrite=1;
    first_writeable=d;
    printf("queueing write, setting first_writeable to %lld\n",d);
  }
#endif
  e->wantwrite=1;
}
