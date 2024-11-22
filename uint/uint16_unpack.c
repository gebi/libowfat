#define NO_UINT16_MACROS
#include "uint16.h"

void uint16_unpack(const char in[2],uint16 *out) {
  *out = (unsigned short)((((unsigned char) in[1]) << 8) + (unsigned char)in[0]);
}
