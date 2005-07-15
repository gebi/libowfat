#include "safemult.h"

int imult32(int32 a,int32 b,int32* c) {
  int64 x=(int64)a*b;
  if ((int32)x != x) return 0;
  *c=x;
  return 1;
}
