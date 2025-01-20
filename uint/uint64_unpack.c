#define NO_UINT64_MACROS
#include <libowfat/uint64.h>

void uint64_unpack(const char in[8],uint64 *out) {
  *out = uint64_read(in);
}
