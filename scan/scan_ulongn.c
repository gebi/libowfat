#include "scan.h"

size_t scan_ulongn(const char* src,size_t n,unsigned long int* dest) {
  register const char *tmp=src;
  register unsigned long int l=0;
  register unsigned char c;
  while (n-->0 && (c=(unsigned char)(*tmp-'0'))<10) {
    unsigned long int n;
    /* we want to do: l=l*10+c
     * but we need to check for integer overflow.
     * to check whether l*10 overflows, we could do
     *   if ((l*10)/10 != l)
     * however, multiplication and division are expensive.
     * so instead of *10 we do (l<<3) (i.e. *8) + (l<<1) (i.e. *2)
     * and check for overflow on all the intermediate steps */
    n=l<<3; if ((n>>3)!=l) break;
    if (n+(l<<1)+c < n) break;
    l=n+(l<<1)+c;
    ++tmp;
  }
  if (tmp-src) *dest=l;
  return (size_t)(tmp-src);
}
