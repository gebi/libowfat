#include <unistd.h>
#include <sys/time.h>
#include <poll.h>
#include <errno.h>
#include "io_internal.h"

int64 io_waituntil2(int64 milliseconds) {
  struct pollfd* p;
  long i,r;
  if (!io_wanted_fds) return 0;
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
  for (i=r-1; i>=0; --i) {
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
