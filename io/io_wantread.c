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
#ifdef __MINGW32__
#include <mswsock.h>
#endif

void io_wantread(int64 d) {
  int newfd;
  io_entry* e=array_get(&io_fds,sizeof(io_entry),d);
  if (!e || e->wantread) return;
  newfd=(!e->wantread && !e->wantwrite);
  io_wanted_fds+=newfd;
#ifdef HAVE_EPOLL
  if (io_waitmode==EPOLL) {
    struct epoll_event x;
    byte_zero(&x,sizeof(x));	// to shut up valgrind
    x.events=EPOLLIN;
    if (e->wantwrite) x.events|=EPOLLOUT;
    x.data.fd=d;
    epoll_ctl(io_master,newfd?EPOLL_CTL_ADD:EPOLL_CTL_MOD,d,&x);
  }
#endif
#ifdef HAVE_KQUEUE
  if (io_waitmode==KQUEUE) {
    struct kevent kev;
    struct timespec ts;
    EV_SET(&kev, d, EVFILT_READ, EV_ADD|EV_ENABLE, 0, 0, 0);
    ts.tv_sec=0; ts.tv_nsec=0;
    kevent(io_master,&kev,1,0,0,&ts);
  }
#endif
#ifdef HAVE_DEVPOLL
  if (io_waitmode==DEVPOLL) {
    struct pollfd x;
    x.fd=d;
    x.events=POLLIN;
    if (e->wantwrite) x.events|=POLLOUT;
    write(io_master,&x,sizeof(x));
  }
#endif
#ifdef HAVE_SIGIO
  if (io_waitmode==_SIGIO) {
    struct pollfd p;
    p.fd=d;
    p.events=POLLIN;
    switch (poll(&p,1,0)) {
    case 1: e->canread=1; break;
    case 0: e->canread=0; break;
    case -1: return;
    }
    if (e->canread) {
      debug_printf(("io_wantread: enqueueing %lld in normal read queue (next is %ld)\n",d,first_readable));
      e->next_read=first_readable;
      first_readable=d;
    }
  }
#endif
#ifdef __MINGW32__
  if (e->listened) {
    if (e->next_accept==0) e->next_accept=socket(AF_INET,SOCK_STREAM,0);
    if (e->next_accept!=-1) {
      AcceptEx(d,e->next_accept,e->inbuf,0,200,200,&e->errorcode,&e->or);
      e->acceptqueued=1;
    }
  } else if (!e->wantread) {
    if (ReadFile((HANDLE)d,e->inbuf,sizeof(e->inbuf),&e->errorcode,&e->or)) {
queueread:
      /* had something to read immediately.  Damn! */
      e->readqueued=0;
      e->canread=1;
      e->bytes_read=e->errorcode;
      e->errorcode=0;
      e->next_read=first_readable;
      first_readable=d;
      return;
    } else if (GetLastError()==ERROR_IO_PENDING)
      e->readqueued=1;
    else
      goto queueread;
#if 0
    e->next_read=first_readable;
    first_readable=d;
#endif
  }
#endif
  e->wantread=1;
}
