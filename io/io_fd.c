#define _GNU_SOURCE
#include <errno.h>
#define my_extern
#include "io_internal.h"
#undef my_extern
#include "byte.h"
#ifdef HAVE_SIGIO
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
#ifdef HAVE_DEVPOLL
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/devpoll.h>
#endif

#ifdef __MINGW32__
#include <stdio.h>
extern HANDLE io_comport;
#endif
array io_fds;
uint64 io_wanted_fds;
array io_pollfds;
enum __io_waitmode io_waitmode;
#if defined(HAVE_KQUEUE) || defined(HAVE_EPOLL) || defined(HAVE_DEVPOLL)
int io_master;
#endif
#if defined(HAVE_SIGIO)
int io_signum;
sigset_t io_ss;

long alt_firstread;
long alt_firstwrite;
#endif

/* put d on internal data structure, return 1 on success, 0 on error */
int io_fd(int64 d) {
  io_entry* e;
#ifndef __MINGW32__
  long r;
  if ((r=fcntl(d,F_GETFL,0)) == -1)
    return 0;	/* file descriptor not open */
#endif
  if (!(e=array_allocate(&io_fds,sizeof(io_entry),d))) return 0;
  if (e->inuse) return 1;
  byte_zero(e,sizeof(io_entry));
  e->inuse=1;
#ifdef __MINGW32__
  e->mh=0;
#else
  if (r&O_NDELAY) e->nonblock=1;
#endif
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
#if defined(HAVE_DEVPOLL)
    if (io_waitmode==UNDECIDED) {
      io_master=open("/dev/poll",O_RDWR);
      if (io_master!=-1) io_waitmode=DEVPOLL;
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
#ifdef __MINGW32__
    io_comport=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);
    if (io_comport) {
      io_waitmode=COMPLETIONPORT;
      fprintf(stderr,"Initialized completion port: %p\n",io_comport);
    } else {
      fprintf(stderr,"ARGH!  Could not init completion port!\n");
      errno=EINVAL;
      return 0;
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
#ifdef __MINGW32__
  if (io_comport) {
    fprintf(stderr,"Queueing %p at completion port %p...",d,io_comport);
    if (CreateIoCompletionPort((HANDLE)d,io_comport,(ULONG_PTR)d,0)==0) {
      fprintf(stderr," failed!\n");
      errno=EBADF;
      return 0;
    }
    fprintf(stderr," OK!\n");
  }
#endif
  return 1;
}
