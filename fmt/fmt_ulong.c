#include "fmt.h"

unsigned int fmt_ulong(char *dest,unsigned long i) {
  register unsigned long len,tmp;
  /* first count the number of bytes needed */
  for (len=1, tmp=i; tmp>9; ++len) tmp/=10;
  if (dest)
    for (tmp=i, dest+=len; tmp; tmp/=10)
      *--dest = (tmp%10)+'0';
  return len;
}
