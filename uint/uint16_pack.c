#define NO_UINT16_MACROS
#include "uint16.h"

void uint16_pack(char *out,uint16 in) {
  out[0]=in&255;
  out[1]=in>>8;
}
