#define NO_UINT64_MACROS
#include <libowfat/uint64.h>
#include <libowfat/uint32.h>

uint64 uint64_read_big(const char in[8]) {
  return ((uint64)uint32_read_big(in)<<32) | uint32_read_big(in+4);
}
