#include <stdlib.h>
#include "iob_internal.h"

int iob_addbuf_internal(io_batch* b,const void* buf,uint64 n,int _free) {
  iob_entry* e;
  if (!n) {
    if (_free) free((char*)buf);
    return 1;
  }
  e=array_allocate(&b->b,sizeof(iob_entry),array_length(&b->b,sizeof(iob_entry)));
  if (!e) return 0;
  e->type=_free?FROMBUF_FREE:FROMBUF;
  e->fd=-1;
  e->buf=buf;
  e->n=n;
  e->offset=0;
  b->bytesleft+=n;
  ++b->bufs;
  return 1;
}
