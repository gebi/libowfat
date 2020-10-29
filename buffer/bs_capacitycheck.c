#include "parse.h"

// This function is supposed to tell the caller if there is more data to
// read. However, we have several limits we could run into. We have our
// own limit, which we check first, but then, if the bytestream is bound
// to an I/O stream we should also try to find out if the I/O stream has
// hit EOF.
int bs_capacitycheck(struct bytestream* bs,size_t capacity) {
  if (bs->cur>=bs->max) return 0;	// if EOF or error, return 0
  if (bs->max - bs->cur < capacity) return 0;	// not EOF but less than that many bytes left
  return 1;
}
