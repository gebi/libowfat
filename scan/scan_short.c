#include "scan.h"

size_t scan_short(const char* src,short* dest) {
  long l;
  size_t len=scan_long(src,&l);
  *dest=l;
  return len;
}
