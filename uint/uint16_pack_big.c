#define NO_UINT16_MACROS
#include <libowfat/uint16.h>

void uint16_pack_big(char out[2],uint16 in) {
  out[0]=(char)(in>>8);
  out[1]=(char)in;
}
