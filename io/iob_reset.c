#include <stdlib.h>
#include "iob_internal.h"

void iob_reset(io_batch* b) {
  array_reset(&b->b);
  free(b);
}
