#include <unistd.h>
#include <sys/time.h>
#ifdef __MINGW32__
#include <windows.h>
#else
#include <poll.h>
#endif
#include <errno.h>
#include "io_internal.h"

#ifdef __MINGW32__
/* All the Unix trickery is unsupported on Windows.  Instead, one is
 * supposed to do the whole write in overlapping mode and then get
 * notified via an I/O completion port when it's done. */

/* So we assume io_trywrite is not used so much and do the overlapping
 * stuff on I/O batches. */

int64 io_trywrite(int64 d,const char* buf,int64 len) {
  io_entry* e=array_get(&io_fds,sizeof(io_entry),d);
  int r;
  if (!e) { errno=EBADF; return -3; }
  if (!e->nonblock) {
    DWORD written;
    if (WriteFile((HANDLE)d,buf,len,&written,0))
      return written;
    else
      return winsock2errno(-3);
  } else {
    if (e->writequeued) {
      errno=EAGAIN;
      return -1;
    }
    if (e->canwrite) {
      e->canwrite=0;
      e->next_write=-1;
      if (e->errorcode) {
	errno=winsock2errno(e->errorcode);
	return -3;
      }
      return e->bytes_written;
    } else {
      if (WriteFile((HANDLE)d,buf,len,&e->errorcode,&e->ow))
	return e->errorcode; /* should not happen */
      else if (GetLastError()==ERROR_IO_PENDING) {
	e->writequeued=1;
	errno=EAGAIN;
	e->errorcode=0;
	return -1;
      } else {
	winsock2errno(-1);
	e->errorcode=errno;
	return -3;
      }
    }
  }
}

#else

int64 io_trywrite(int64 d,const char* buf,int64 len) {
  long r;
  struct itimerval old,new;
  struct pollfd p;
  io_entry* e=array_get(&io_fds,sizeof(io_entry),d);
  io_sigpipe();
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
    new.it_interval.tv_usec=10000;
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

#endif
