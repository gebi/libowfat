#include "io.h"
#include "array.h"

typedef struct {
  unsigned int wantread:1;
  unsigned int wantwrite:1;
  unsigned int canread:1;
  unsigned int canwrite:1;
  unsigned int nonblock:1;
  unsigned int inuse:1;
  tai6464 timeout;
  long next_read;
  long next_write;
} io_entry;

array io_fds;
uint64 io_wanted_fds;
array io_pollfds;

unsigned long first_readable;
unsigned long first_writeable;

int64 io_waituntil2(int64 milliseconds);
