#define NO_UINT16_MACROS
#include "uint16.h"

void uint16_pack_big(char *out,uint16 in) {
  out[0]=in>>8;
  out[1]=in&255;
}
