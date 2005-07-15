#include "safemult.h"

int imult16(int16 a,int16 b,int16* c) {
  int32 x=(int32)a*b;
  if ((int16)x != x) return 0;
  *c=x;
  return 1;
}
