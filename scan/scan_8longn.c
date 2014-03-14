#include "scan.h"

size_t scan_8longn(const char *src,size_t n,unsigned long *dest) {
  register const char *tmp=src;
  register unsigned long l=0;
  register unsigned char c;
  while (n-->0 && (c=*tmp-'0')<8) {
    if (l>>(sizeof(l)*8-3)) break;
    l=l*8+c;
    ++tmp;
  }
  *dest=l;
  return tmp-src;
}
