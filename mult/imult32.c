#include "safemult.h"

int imult32(int32 a,int32 b,int32* c) {
  int neg=(a<0);
  uint32 d;
  if (neg) a=-a;
  if (b<0) { neg^=1; b=-b; }
  if (umult32(a,b,&d)) return 0;
  *c=(neg?-d:d);
  return 1;
}
