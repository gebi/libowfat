#include "scan.h"

size_t scan_8long(const char *src,unsigned long *dest) {
  register const char *tmp=src;
  register unsigned long l=0;
  register unsigned char c;
  while ((c=*tmp-'0')<8) {
    l=l*8+c;
    ++tmp;
  }
  *dest=l;
  return tmp-src;
}
