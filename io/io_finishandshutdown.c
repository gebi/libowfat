#include "io_internal.h"

void io_finishandshutdown(void) {
  array_reset(&io_fds);
  array_reset(&io_pollfds);
}
