#include <unistd.h>
#include <sys/time.h>
#include <poll.h>
#include <errno.h>
#include "io_internal.h"

int64 io_tryread(int64 d,char* buf,int64 len) {
  long r;
  struct itimerval old,new;
  struct pollfd p;
  io_entry* e=array_get(&io_fds,sizeof(io_entry),d);
  if (!e) { errno=EBADF; return -3; }
  if (!e->nonblock) {
    p.fd=d;
    if (p.fd != d) { errno=EBADF; return -3; }	/* catch overflow */
    p.events=POLLIN;
    switch (poll(&p,1,0)) {
    case -1: return -3;
    case 0: errno=EAGAIN;
	    e->canread=0;
	    e->next_read=-1;
	    return -1;
    }
    new.it_interval.tv_usec=0;
    new.it_interval.tv_sec=0;
    new.it_value.tv_usec=10000;
    new.it_value.tv_sec=0;
    setitimer(ITIMER_REAL,&new,&old);
  }
  r=read(d,buf,len);
  if (!e->nonblock) {
    new.it_interval.tv_usec=0;
    new.it_interval.tv_sec=0;
    new.it_value.tv_usec=0;
    new.it_value.tv_sec=0;
    setitimer(ITIMER_REAL,&new,&old);
  }
  if (r==-1) {
    if (errno==EINTR) errno=EAGAIN;
    if (errno!=EAGAIN)
      r=-3;
  }
  if (r==-1 || r==0) {
    e->canread=0;
#ifdef HAVE_SIGIO
    if (d==alt_firstread) {
      debug_printf(("io_tryread: dequeueing %ld from alt read queue (next is %ld)\n",d,e->next_read));
      alt_firstread=e->next_read;
    }
#endif
    e->next_read=-1;
  }
  return r;
}
