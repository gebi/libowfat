#include <unistd.h>
#include <sys/time.h>
#include <poll.h>
#include <errno.h>
#include "io_internal.h"
#ifdef HAVE_KQUEUE
#include <sys/event.h>
#endif
#ifdef HAVE_EPOLL
#include <sys/epoll.h>
#endif

int64 io_waituntil2(int64 milliseconds) {
  struct pollfd* p;
  long i,j,r;
  if (!io_wanted_fds) return 0;
#ifdef HAVE_EPOLL
  if (io_waitmode==EPOLL) {
    int n;
    struct epoll_event y[100];
    if ((n=epoll_wait(io_master,y,100,milliseconds))==-1) return -1;
    for (i=n-1; i>=0; --i) {
      io_entry* e=array_get(&io_fds,sizeof(io_entry),y[i].data.fd);
      if (e) {
	if (y[i].events&EPOLLIN) {
	  e->canread=1;
	  e->next_read=first_readable;
	  first_readable=y[i].data.fd;
	}
	if (y[i].events&EPOLLOUT) {
	  e->canwrite=1;
	  e->next_write=first_writeable;
	  first_writeable=y[i].data.fd;
	}
      }
    }
    return n;
  }
#endif
#ifdef HAVE_KQUEUE
  if (io_waitmode==KQUEUE) {
    struct kevent y[100];
    int n;
    struct timespec ts;
    ts.tv_sec=milliseconds/1000; ts.tv_nsec=(milliseconds%1000)*1000000;
    if ((n=kevent(io_master,0,0,y,100,&ts))==-1) return -1;
    for (i=n-1; i>=0; --i) {
      io_entry* e=array_get(&io_fds,sizeof(io_entry),y[--n].ident);
      if (e) {
	if (y[n].filter==EVFILT_READ) {
	  e->canread=1;
	  e->next_read=first_readable;
	  first_readable=y[n].ident;
	}
	if (y[n].filter==EVFILT_WRITE) {
	  e->canwrite=1;
	  e->next_write=first_writeable;
	  first_writeable=y[i].ident;
	}
      }
    }
    return n;
  }
#endif
  for (i=r=0; i<array_length(&io_fds,sizeof(io_entry)); ++i) {
    io_entry* e=array_get(&io_fds,sizeof(io_entry),i);
    if (!e) return -1;
    e->canread=e->canwrite=0;
    if (e->wantread || e->wantwrite) {
      struct pollfd* p;
      if ((p=array_allocate(&io_pollfds,sizeof(struct pollfd),r))) {
	p->fd=i;
	p->events=(e->wantread?POLLIN:0) + (e->wantwrite?POLLOUT:0);
	++r;
      } else
	return -1;
    }
  }
  p=array_start(&io_pollfds);
again:
  while ((i=poll(array_start(&io_pollfds),r,milliseconds))==0);
  if (i==-1) {
    if (errno==EINTR) goto again;
    return -1;
  }
  for (j=r-1; j>=0; --j) {
    io_entry* e=array_get(&io_fds,sizeof(io_entry),p->fd);
    if (p->revents&POLLIN) {
      e->canread=1;
      e->next_read=first_readable;
      first_readable=p->fd;
    }
    if (p->revents&POLLOUT) {
      e->canwrite=1;
      e->next_write=first_writeable;
      first_writeable=p->fd;
    }
    p++;
  }
  return i;
}
