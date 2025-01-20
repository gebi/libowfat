#define NO_UINT16_MACROS
#include <libowfat/uint16.h>

uint16 uint16_read_big(const char in[2]) {
  return (unsigned short)((((unsigned char) in[0]) << 8) + (unsigned char)in[1]);
}
