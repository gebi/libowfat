#include <stdlib.h>
#include <sys/mman.h>
#include "byte.h"
#include "iob_internal.h"

void iob_reset(io_batch* b) {
  unsigned long i,l;
  iob_entry* x=array_start(&b->b);
  l=array_length(&b->b,sizeof(iob_entry));
  for (i=0; i<l; ++i) {
    switch (x[i].type) {
    case FROMBUF_FREE:
      free((char*)x[i].buf);
      break;
    case FROMBUF_MUNMAP:
      munmap((char*)x[i].buf,x[i].n);
      break;
    case FROMFILE_CLOSE:
      io_close(x[i].fd);
    default:
      break;
    }
  }
  array_reset(&b->b);
  byte_zero(b,sizeof(*b));
}
