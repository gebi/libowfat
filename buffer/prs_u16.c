#include "parse.h"

uint16_t prs_u16(struct bytestream* bs) {
  return bs_get(bs) | (bs_get(bs) << 8);
}

#ifdef UNITTEST
#include <assert.h>

int main() {
  struct bytestream bs = BS_FROM_MEMBUF("\x34\x12",2);
  assert(prs_u16(&bs) == 0x1234);
  assert(bs_err(&bs) == 0);
  assert(prs_u16(&bs) == 0);
  assert(bs_err(&bs));
}
#endif
