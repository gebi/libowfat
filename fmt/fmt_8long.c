#include "fmt.h"

unsigned int fmt_8long(char *dest,unsigned long i) {
  register unsigned long len,tmp;
  /* first count the number of bytes needed */
  for (len=1, tmp=i; tmp>7; ++len) tmp/=8;
  if (dest)
    for (tmp=i, dest+=len; tmp; tmp/=8)
      *--dest = (tmp&7)+'0';
  return len;
}
