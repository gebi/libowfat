#define NO_UINT16_MACROS
#include <libowfat/uint16.h>

void uint16_pack(char out[2],uint16 in) {
  out[0]=(char)in;
  out[1]=(char)(in>>8);
}
