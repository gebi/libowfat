#include "safemult.h"

int imult64(int64 a,int64 b,int64* c) {
  int neg=(a<0);
  uint64 d;
  if (neg) a=-a;
  if (b<0) { neg^=1; b=-b; }
  if (umult64(a,b,&d)) return 0;
  *c=(neg?-d:d);
  return 1;
}

