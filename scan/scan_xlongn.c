#include "scan.h"

size_t scan_xlongn(const char *src,size_t n,unsigned long *dest) {
  register const char *tmp=src;
  register unsigned long l=0;
  register unsigned char c;
  while (n-->0 && (l>>(sizeof(l)*8-4))==0 && (c=scan_fromhex(*tmp))<16) {
    l=(l<<4)+c;
    ++tmp;
  }
  *dest=l;
  return tmp-src;
}
