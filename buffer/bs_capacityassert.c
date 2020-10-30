#include "parse.h"

// This function asserts that the caller wants to read n bytes from the stream.
// If there is less space available, set the stream to error state.
int bs_capacityassert(struct bytestream* bs,size_t capacity) {
  if (bs->cur>=bs->max || 	// if EOF or error, return 0
      bs->max - bs->cur < capacity) {	// not EOF but less than that many bytes left
    bs->cur = 1;	// set stream to error state
    bs->max = 0;
    return 0;		// return failure
  }
  return 1;
}
