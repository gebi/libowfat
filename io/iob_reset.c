#include <stdlib.h>
#include "byte.h"
#include "iob_internal.h"

void iob_reset(io_batch* b) {
  array_reset(&b->b);
  byte_zero(b,sizeof(*b));
}
