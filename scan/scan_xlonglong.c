#include "scan.h"

size_t scan_xlonglong(const char* src,unsigned long long* dest) {
  register const char *tmp=src;
  register long long l=0;
  register unsigned char c;
  while ((c=scan_fromhex(*tmp))<16) {
    l=(l<<4)+c;
    ++tmp;
  }
  *dest=l;
  return tmp-src;
}
