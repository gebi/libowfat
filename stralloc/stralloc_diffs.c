#include "stralloc.h"
#include "byte.h"
#include "str.h"

extern int stralloc_diffs(const stralloc* a,const unsigned char* b) {
  register unsigned long int i;
  register int j;
  for (i=0;;++i) {
    if (i==a->len) return (!b[i])?0:-1; if (!b[i]) return 1;
    if ((j=(a->s[i]-b[i]))) return j;
  }
  return j;
}

