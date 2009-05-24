#include "io_internal.h"
#ifdef HAVE_EPOLL
#define _XOPEN_SOURCE
#endif
#ifdef HAVE_SIGIO
#define _GNU_SOURCE
#include <signal.h>
#endif
#include <unistd.h>
#include <sys/time.h>
#ifdef __MINGW32__
#include <windows.h>
#include <stdio.h>
#else
#include <poll.h>
#endif
#include <errno.h>
#ifdef HAVE_KQUEUE
#include <sys/event.h>
#endif
#ifdef HAVE_EPOLL
#include <inttypes.h>
#include <sys/epoll.h>
#endif
#ifdef HAVE_DEVPOLL
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/devpoll.h>
#endif

#ifdef DEBUG
#include <stdio.h>
#endif

int64 io_waituntil2(int64 milliseconds) {
#ifndef __MINGW32__
  struct pollfd* p;
#endif
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
	if (y[i].events&(EPOLLERR|EPOLLHUP)) {
	  /* error; signal whatever app is looking for */
	  if (e->wantread) y[i].events|=EPOLLIN;
	  if (e->wantwrite) y[i].events|=EPOLLOUT;
	}
#ifdef EPOLLRDNORM
	if (!e->canread && (y[i].events&(EPOLLIN|EPOLLPRI|EPOLLRDNORM|EPOLLRDBAND))) {
#else
	if (!e->canread && (y[i].events&(EPOLLIN|EPOLLPRI))) {
#endif
	  e->canread=1;
	  e->next_read=first_readable;
	  first_readable=y[i].data.fd;
	}
	if (!e->canwrite && (y[i].events&EPOLLOUT)) {
	  e->canwrite=1;
	  e->next_write=first_writeable;
	  first_writeable=y[i].data.fd;
	}
      } else {
	epoll_ctl(io_master,EPOLL_CTL_DEL,y[i].data.fd,y+i);
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
    if ((n=kevent(io_master,0,0,y,100,milliseconds!=-1?&ts:0))==-1) return -1;
    for (i=n-1; i>=0; --i) {
      io_entry* e=array_get(&io_fds,sizeof(io_entry),y[--n].ident);
#ifdef DEBUG
      if (!e) {
	e=e;
      }
#endif
      if (e) {
	if (y[n].flags&EV_ERROR) {
	  /* error; signal whatever app is looking for */
	  if (e->wantread) y[n].filter=EVFILT_READ; else
	  if (e->wantwrite) y[n].filter=EVFILT_WRITE;
	}
	if (!e->canread && (y[n].filter==EVFILT_READ)) {
	  e->canread=1;
	  e->next_read=first_readable;
	  first_readable=y[n].ident;
	}
	if (!e->canwrite && (y[n].filter==EVFILT_WRITE)) {
	  e->canwrite=1;
	  e->next_write=first_writeable;
	  first_writeable=y[i].ident;
	}
#ifdef DEBUG
      } else {
	fprintf(stderr,"got kevent on fd#%d, which is not in array!\n",y[n].ident);
#endif
      }
    }
    return n;
  }
#endif
#ifdef HAVE_DEVPOLL
  if (io_waitmode==DEVPOLL) {
    dvpoll_t timeout;
    struct pollfd y[100];
    int n;
    timeout.dp_timeout=milliseconds;
    timeout.dp_nfds=100;
    timeout.dp_fds=y;
    if ((n=ioctl(io_master,DP_POLL,&timeout))==-1) return -1;
    for (i=n-1; i>=0; --i) {
      io_entry* e=array_get(&io_fds,sizeof(io_entry),y[--n].fd);
      if (e) {
	if (y[n].revents&(POLLERR|POLLHUP|POLLNVAL)) {
	  /* error; signal whatever app is looking for */
	  if (e->wantread) y[n].revents=POLLIN;
	  if (e->wantwrite) y[n].revents=POLLOUT;
	}
	if (!e->canread && (y[n].revents&POLLIN)) {
	  e->canread=1;
	  if (e->next_read==-1) {
	    e->next_read=first_readable;
	    first_readable=y[n].fd;
	  }
	}
	if (!e->canwrite && (y[n].revents&POLLOUT)) {
	  e->canwrite=1;
	  if (e->next_write==-1) {
	    e->next_write=first_writeable;
	    first_writeable=y[i].fd;
	  }
	}
#ifdef DEBUG
      } else {
	fprintf(stderr,"got kevent on fd#%d, which is not in array!\n",y[n].fd);
#endif
      }
    }
    return n;
  }
#endif
#ifdef HAVE_SIGIO
  if (io_waitmode==_SIGIO) {
    siginfo_t info;
    struct timespec ts;
    int r;
    io_entry* e;
    if (alt_firstread>=0 && (e=array_get(&io_fds,sizeof(io_entry),alt_firstread)) && e->canread) return 1;
    if (alt_firstwrite>=0 && (e=array_get(&io_fds,sizeof(io_entry),alt_firstwrite)) && e->canwrite) return 1;
    if (milliseconds==-1)
      r=sigwaitinfo(&io_ss,&info);
    else {
      ts.tv_sec=milliseconds/1000; ts.tv_nsec=(milliseconds%1000)*1000000;
      r=sigtimedwait(&io_ss,&info,&ts);
    }
    switch (r) {
    case SIGIO:
      /* signal queue overflow */
      signal(io_signum,SIG_DFL);
      goto dopoll;
    default:
      if (r==io_signum) {
	io_entry* e=array_get(&io_fds,sizeof(io_entry),info.si_fd);
	if (e) {
	  if (info.si_band&(POLLERR|POLLHUP)) {
	    /* error; signal whatever app is looking for */
	    if (e->wantread) info.si_band|=POLLIN;
	    if (e->wantwrite) info.si_band|=POLLOUT;
	  }
	  if (info.si_band&POLLIN && !e->canread) {
	    debug_printf(("io_waituntil2: enqueueing %ld in normal read queue before %ld\n",info.si_fd,first_readable));
	    e->canread=1;
	    e->next_read=first_readable;
	    first_readable=info.si_fd;
	  }
	  if (info.si_band&POLLOUT && !e->canwrite) {
	    debug_printf(("io_waituntil2: enqueueing %ld in normal write queue before %ld\n",info.si_fd,first_writeable));
	    e->canwrite=1;
	    e->next_write=first_writeable;
	    first_writeable=info.si_fd;
	  }
#ifdef DEBUG
	} else {
	  fprintf(stderr,"got kevent on fd#%d, which is not in array!\n",info.si_fd);
#endif
	}
      }
    }
    return 1;
  }
dopoll:
#endif
#ifdef __MINGW32__
  DWORD numberofbytes;
  ULONG_PTR x;
  LPOVERLAPPED o;
  if (first_readable!=-1 || first_writeable!=-1) {
    fprintf(stderr,"io_waituntil2() returning immediately because first_readable(%p) or first_writeable(%p) are set\n",first_readable,first_writeable);
    return;
  }
  fprintf(stderr,"Calling GetQueuedCompletionStatus %p...",io_comport);
  if (GetQueuedCompletionStatus(io_comport,&numberofbytes,&x,&o,milliseconds==-1?INFINITE:milliseconds)) {
    io_entry* e=array_get(&io_fds,sizeof(io_entry),x);
    fprintf(stderr," OK.  Got %x, e=%p\n",x,e);
    if (!e) return 0;
    e->errorcode=0;
    fprintf(stderr,"o=%p, e->or=%p, e->ow=%p, e->os=%p\n",o,&e->or,&e->ow,&e->os);
    fprintf(stderr,"e->readqueued=%d, e->writequeued=%d, e->acceptqueued=%d, e->connectqueued=%d, e->sendfilequeued=%d\n",
	    e->readqueued,e->writequeued,e->acceptqueued,e->connectqueued,e->sendfilequeued);
    if (o==&e->or && e->readqueued==1) {
      e->readqueued=2;
      e->canread=1;
      e->bytes_read=numberofbytes;
      e->next_read=first_readable;
      first_readable=x;
//      printf("read %lu bytes on fd %lu: %p\n",numberofbytes,x,e);
    } else if (o==&e->ow && e->writequeued==1) {
      e->writequeued=2;
      e->canwrite=1;
      e->bytes_written=numberofbytes;
      e->next_write=first_writeable;
      first_writeable=x;
    } else if (o==&e->or && e->acceptqueued==1) {
      e->acceptqueued=2;
      e->canread=1;
      e->next_read=first_readable;
      first_readable=x;
    } else if (o==&e->ow && e->connectqueued==1) {
      e->connectqueued=2;
      e->canwrite=1;
      e->next_write=first_writeable;
      first_writeable=x;
    } else if (o==&e->os && e->sendfilequeued==1) {
      e->sendfilequeued=2;
      e->canwrite=1;
      e->bytes_written=numberofbytes;
      e->next_write=first_writeable;
      first_writeable=x;
    }
    return 1;
  } else {
    /* either the overlapped I/O request failed or we timed out */
    DWORD err;
    io_entry* e;
    fprintf(stderr," failure, o=%p.\n",o);
    if (!o) return 0;	/* timeout */
    /* we got a completion packet for a failed I/O operation */
    err=GetLastError();
    if (err==WAIT_TIMEOUT) return 0;	/* or maybe not */
    e=array_get(&io_fds,sizeof(io_entry),x);
    if (!e) return 0;	/* WTF?! */
    e->errorcode=err;
    if (o==&e->or && (e->readqueued || e->acceptqueued)) {
      if (e->readqueued) e->readqueued=2; else
      if (e->acceptqueued) e->acceptqueued=2;
      e->canread=1;
      e->bytes_read=-1;
      e->next_read=first_readable;
      first_readable=x;
    } else if ((o==&e->ow || o==&e->os) &&
               (e->writequeued || e->connectqueued || e->sendfilequeued)) {
      if (o==&e->ow) {
	if (e->writequeued) e->writequeued=2; else
        if (e->connectqueued) e->connectqueued=2;
      } else if (o==&e->os) e->sendfilequeued=2;
      e->canwrite=1;
      e->bytes_written=-1;
      e->next_write=first_writeable;
      first_writeable=x;
    }
    return 1;
  }
#else
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
  if ((i=poll(array_start(&io_pollfds),r,milliseconds))<1) return -1;
  for (j=r-1; j>=0; --j) {
    io_entry* e=array_get(&io_fds,sizeof(io_entry),p->fd);
    if (p->revents&(POLLERR|POLLHUP|POLLNVAL)) {
      /* error; signal whatever app is looking for */
      if (e->wantread) p->revents|=POLLIN;
      if (e->wantwrite) p->revents|=POLLOUT;
    }
    if (!e->canread && (p->revents&POLLIN)) {
      e->canread=1;
      e->next_read=first_readable;
      first_readable=p->fd;
    }
    if (!e->canwrite && (p->revents&POLLOUT)) {
      e->canwrite=1;
      e->next_write=first_writeable;
      first_writeable=p->fd;
    }
    p++;
  }
  return i;
#endif
}
