#include "iob_internal.h"

int iob_addfile(io_batch* b,int64 fd,uint64 off,uint64 n) {
  io_entry* e=array_allocate(&b->b,sizeof(io_entry),
			     array_length(&b->b,sizeof(io_entry)));
  if (!e) return 0;
  e->type=FROMFILE;
  e->fd=fd;
  e->buf=0;
  e->n=n;
  e->offset=off;
  b->bytesleft+=n;
  ++b->files;
  return 1;
}
