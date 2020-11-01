#include "parse.h"

void bs_seterrorstate(struct bytestream* b) {
  b->cur = 1;
  b->max = 0;
}
