#include "safemult.h"

int imult16(int16 a,int16 b,int16* c) {
  int neg=(a<0);
  if (neg) a=-a;
  if (b<0) { neg^=1; b=-b; }
  if (umult16(a,b,c)) return 0;
  if (neg) *c=-*c;
  return 1;
}
