#include "scan.h"

size_t scan_xshort(const char* src,unsigned short* dest) {
  register const char *tmp=src;
  register unsigned short l=0;
  register unsigned char c;
  while ((l>>(sizeof(l)*8-4))==0 && (c=scan_fromhex(*tmp))<16) {
    l=(l<<4)+c;
    ++tmp;
  }
  *dest=l;
  return tmp-src;
}
