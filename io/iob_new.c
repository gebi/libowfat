#include <stdlib.h>
#include "iob_internal.h"

io_batch* iob_new(int hint_entries) {
  io_batch* b=(io_batch*)malloc(sizeof(io_batch));
  if (!b) return 0;
  if (hint_entries) {
    if (!array_allocate(&b->b,sizeof(io_entry),hint_entries)) {
      free(b);
      return 0;
    }
    array_trunc(&b->b);
  }
  b->next=b->bufs=b->files=0;
  b->bytesleft=0;
  return b;
}
