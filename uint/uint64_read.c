#define NO_UINT64_MACROS
#include <libowfat/uint64.h>
#include <libowfat/uint32.h>

uint64 uint64_read(const char in[8]) {
  return uint32_read(in) | ((uint64)uint32_read(in+4)<<32);
}
