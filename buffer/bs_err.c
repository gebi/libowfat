#include "parse.h"

int bs_err(struct bytestream* bs) {
  return (bs->cur > bs->max);
}

