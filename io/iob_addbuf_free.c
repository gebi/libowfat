#include <stdlib.h>
#include "iob_internal.h"

static void cleanup(struct iob_entry* x) {
  free((char*)x->buf);
}

int iob_addbuf_free(io_batch* b,const void* buf,uint64 n) {
  return iob_addbuf_internal(b,buf,n,cleanup);
}
