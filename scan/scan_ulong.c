#include "scan.h"

size_t scan_ulong(const char* src,unsigned long int* dest) {
  register const char *tmp=src;
  register unsigned long int l=0;
  register unsigned char c;
  while ((c=*tmp-'0')<10) {
    unsigned long int n;
    /* division is very slow on most architectures */
    n=l<<3; if ((n>>3)!=l) break;
    if (n+(l<<1) < n) break;
    n+=l<<1;
    if (n+c < n) break;
    l=n+c;
    ++tmp;
  }
  if (tmp-src) *dest=l;
  return tmp-src;
}
