#include "safemult.h"

int umult32(uint32 a,uint32 b,uint32* c) {
  unsigned long long x=(unsigned long long)a*b;
  if (x>0xffffffff) return 0;
  *c=x&0xffffffff;
  return 1;
}
