#include "parse.h"

void bs_init_bstream_size(struct bytestream* bs,struct bytestream* other,size_t maxlen) {
  bs->type = BSTREAM;
  // check if we have enough capacity in the parent bytestream
  if (bs_capacitycheck(other, maxlen)) {
    bs->cur = 0;
    bs->max = maxlen;
  } else {
    // nope, so set the new stream to error state right out of the box
    bs->cur = 1;
    bs->max = 0;
  }
  bs->u.bs=other;
}
