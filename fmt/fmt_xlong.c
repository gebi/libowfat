#include "fmt.h"

static inline char tohex(char c) {
  return c>=10?c-10+'a':c+'0';
}

unsigned int fmt_xlong(char *dest,unsigned long i) {
  register unsigned long len,tmp;
  /* first count the number of bytes needed */
  for (len=1, tmp=i; tmp>15; ++len) tmp>>=4;
  if (dest)
    for (tmp=i, dest+=len; tmp; tmp>>=4)
      *--dest = tohex(tmp&15);
  return len;
}
