#include "iob_internal.h"

int iob_addbuf(io_batch* b,const void* buf,uint64 n) {
  io_entry* e=array_allocate(&b->b,sizeof(io_entry),
			     array_length(&b->b,sizeof(io_entry)));
  if (!e) return 0;
  e->type=FROMBUF;
  e->fd=-1;
  e->buf=buf;
  e->n=n;
  e->offset=0;
  b->bytesleft+=n;
  ++b->bufs;
  return 1;
}
