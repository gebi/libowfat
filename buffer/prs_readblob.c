#include "parse.h"

static const size_t max_ssize_t = (((size_t)1) << (sizeof(size_t)*8-1))-1;

/* Read n bytes from stream. Return n.
 * Set stream to error state if not enough space or I/O error. */
ssize_t prs_readblob(struct bytestream* bs,unsigned char* dest,size_t destlen) {
  ssize_t r;
  if (destlen > max_ssize_t)
    destlen = max_ssize_t;
  if (!bs_capacityassert(bs,destlen)) return -1;
  r = destlen;

  switch (bs->type) {

  case MEMBUF:
    memcpy(dest, bs->u.base+bs->cur, destlen);
    bs->cur += destlen;
    break;

  case IOBUF:
    buffer_get(bs->u.b, (char*)dest, destlen);
    break;

  case BSTREAM:
    bs->cur += destlen;
    r=prs_readblob(bs->u.bs, dest, destlen);
    if (r != (ssize_t)destlen) {
      bs->cur = 1;	// set bytestream to error state
      bs->max = 0;
      return -1;
    }
    break;

  }

    return r;
}
