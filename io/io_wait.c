#include <unistd.h>
#include <sys/time.h>
#include <poll.h>
#include <errno.h>
#include "io_internal.h"

void io_wait() {
  struct pollfd* p;
  long i,r;
  if (!io_wanted_fds) return;
  for (i=r=0; i<array_length(&io_fds,sizeof(io_entry)); ++i) {
    io_entry* e=array_get(&io_fds,sizeof(io_entry),i);
    if (!e) return;
    e->canread=e->canwrite=0;
    if (e->wantread || e->wantwrite) {
      struct pollfd* p;
      if ((p=array_allocate(&io_pollfds,sizeof(struct pollfd),r))) {
	p->fd=i;
	p->events=(e->wantread?POLLIN:0) + (e->wantwrite?POLLOUT:0);
	++r;
      } else
	return;
    }
  }
  p=array_start(&io_pollfds);
  while ((i=poll(array_start(&io_pollfds),r,99999999))==0);
  if (i==-1) return;
  for (i=0; i<r; ++i) {
    io_entry* e=array_get(&io_fds,sizeof(io_entry),p->fd);
    if (p->revents&POLLIN) e->canread=1;
    if (p->revents&POLLOUT) e->canwrite=1;
    p++;
  }
}
