#include "safemult.h"

int imult32(int32 a,int32 b,int32* c) {
  int neg=(a<0);
  if (neg) a=-a;
  if (b<0) { neg^=1; b=-b; }
  if (umult32(a,b,c)) return 0;
  if (neg) *c=-*c;
  return 1;
}
