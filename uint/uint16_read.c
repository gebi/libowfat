#define NO_UINT16_MACROS
#include <libowfat/uint16.h>

uint16 uint16_read(const char in[2]) {
  return (unsigned short)((((unsigned char) in[1]) << 8) | (unsigned char)in[0]);
}
