#include "scan.h"

size_t scan_8longlong(const char *src,unsigned long long *dest) {
  register const char *tmp=src;
  register unsigned long long l=0;
  register unsigned char c;
  while ((c=*tmp-'0')<8) {
    if (l>>(sizeof(l)*8-3)) break;
    l=l*8+c;
    ++tmp;
  }
  *dest=l;
  return tmp-src;
}
