#include "scan.h"

size_t scan_xint(const char* src,unsigned int* dest) {
  unsigned long l;
  register int len=scan_xlong(src,&l);
  *dest=l;
  return len;
}
