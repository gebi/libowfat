#include "scan.h"

size_t scan_8short(const char* src,unsigned short* dest) {
  unsigned long l;
  size_t len=scan_8long(src,&l);
  *dest=l;
  return len;
}
