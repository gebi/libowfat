#include "scan.h"

size_t scan_xshort(const char* src,unsigned short* dest) {
  unsigned long l;
  size_t len=scan_xlong(src,&l);
  *dest=l;
  return len;
}
