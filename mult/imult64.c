#include "safemult.h"

int imult64(int64 a,int64 b,int64* c) {
  int neg=(a<0);
  if (neg) a=-a;
  if (b<0) { neg^=1; b=-b; }
  if (umult64(a,b,c)) return 0;
  if (neg) *c=-*c;
  return 1;
}

