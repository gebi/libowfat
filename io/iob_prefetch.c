#include "iob_internal.h"
#include <stdio.h>
#include <sys/mman.h>

void iob_prefetch(io_batch* b,uint64 bytes) {
  iob_entry* e,* last;
  if (b->bytesleft==0) return;
  last=(iob_entry*)(((char*)array_start(&b->b))+array_bytes(&b->b));
  e=(iob_entry*)array_start(&b->b);
  if (!e) return;
  for (; e<last; ++e) {
    if (e->type==FROMFILE) {
      char* c,* d;
      if (e->n>=1000000) {
	d=c=mmap(0,bytes,PROT_READ,MAP_SHARED,e->fd,(e->offset|4095)+1);
	if (c!=MAP_FAILED) {
	  while (bytes>4096) {
	    volatile char x=*d;
	    bytes-=4096;
	    d+=4096;
	    (void)x;
	  }
	}
	munmap(c,bytes);
      }
      return;
    }
  }
}
