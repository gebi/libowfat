#include <unistd.h>
#include <sys/time.h>
#include <poll.h>
#include <errno.h>
#include "io_internal.h"

int64 io_trywrite(int64 d,const char* buf,int64 len) {
  long r;
  struct itimerval old,new;
  struct pollfd p;
  io_entry* e=array_get(&io_fds,sizeof(io_entry),d);
  if (!e) { errno=EBADF; return -3; }
  if (!e->nonblock) {
    p.fd=d;
    if (p.fd != d) { errno=EBADF; return -3; }	/* catch overflow */
    p.events=POLLOUT;
    switch (poll(&p,1,0)) {
    case -1: return -3;
    case 0: errno=EAGAIN;
	    e->canwrite=0;
	    e->next_write=-1;
	    return -1;
    }
    new.it_interval.tv_usec=0;
    new.it_interval.tv_sec=0;
    new.it_value.tv_usec=10000;
    new.it_value.tv_sec=0;
    setitimer(ITIMER_REAL,&new,&old);
  }
  r=write(d,buf,len);
  if (!e->nonblock) {
    setitimer(ITIMER_REAL,&old,0);
  }
  if (r==-1) {
    if (errno==EINTR) errno=EAGAIN;
    if (errno!=EAGAIN)
      r=-3;
  }
  if (r==-1 || r==0) {
    e->canwrite=0;
#ifdef HAVE_SIGIO
    if (d==alt_firstwrite) {
      debug_printf(("io_trywrite: dequeueing %ld from alt write queue (next is %ld)\n",d,e->next_write));
      alt_firstwrite=e->next_write;
    }
#endif
    e->next_write=-1;
  }
  return r;
}
