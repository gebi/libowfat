#include "iob.h"
#include "array.h"

typedef struct io_entry {
  enum { FROMBUF, FROMFILE } type;
  int64 fd;
  const char* buf;
  uint64 offset,n;
} io_entry;
