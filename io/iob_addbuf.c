#include "iob_internal.h"

int iob_addbuf(io_batch* b,const void* buf,uint64 n) {
  iob_entry* e=array_allocate(&b->b,sizeof(iob_entry),
			     array_length(&b->b,sizeof(iob_entry)));
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
