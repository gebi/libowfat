#include "iob.h"
#include "array.h"

struct io_batch {
  array b;
  uint64 bytesleft;
  long next,bufs,files;
};

typedef struct io_entry {
  enum { FROMBUF, FROMFILE } type;
  int64 fd;
  const char* buf;
  uint64 offset,n;
} io_entry;
