#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include "io_internal.h"

int64 io_waitread(int64 d,char* buf,int64 len) {
  long r;
  struct pollfd p;
  io_entry* e=array_get(&io_fds,sizeof(io_entry),d);
  if (!e) { errno=EBADF; return -3; }
  if (e->nonblock) {
again:
    p.fd=d;
    if (p.fd != d) { errno=EBADF; return -3; }	/* catch overflow */
    p.events=POLLIN;
    switch (poll(&p,1,-1)) {
    case -1: if (errno==EAGAIN) goto again; return -3;
    }
  }
  r=read(d,buf,len);
  if (r==-1) {
    if (errno==EAGAIN)
      goto again;
    r=-3;
  }
  return r;
}
