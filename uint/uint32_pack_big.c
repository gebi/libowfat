#define NO_UINT32_MACROS
#include "uint32.h"

void uint32_pack_big(char *out,uint32 in) {
  out[3]=in&0xff; in>>=8;
  out[2]=in&0xff; in>>=8;
  out[1]=in&0xff; in>>=8;
  out[0]=in&0xff;
}
