#include "safemult.h"

/* return 1 for overflow, 0 for ok */
int umult64(uint64 a,uint64 b,uint64* c) {
  uint32 ahi=a>>32;
  uint32 alo=(a&0xffffffff);
  uint32 bhi=b>>32;
  uint32 blo=(b&0xffffffff);

  // a=ahi*x+alo, b=bhi*x+blo
  // a*b = (ahi*x+alo) * (bhi*x+blo)
  //     = ahi*x*bhi*x + ahi*x*blo + alo*bhi*x + alo*blo

  // -> overflow if ahi*bhi != zero */
  if (ahi && bhi) return 0;

  a=(uint64)(ahi)*blo+(uint64)(alo)*bhi;
  if (a>0xffffffff) return 0;
  *c=(a<<32)+(uint64)(alo)*blo;
  return 1;
}

