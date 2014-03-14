#include "scan.h"

size_t scan_8int(const char* src,unsigned int* dest) {
  register const char *tmp=src;
  register unsigned int l=0;
  register unsigned char c;
  while ((c=*tmp-'0')<8) {
    if (l>>(sizeof(l)*8-3)) break;
    l=l*8+c;
    ++tmp;
  }
  *dest=l;
  return tmp-src;
}
