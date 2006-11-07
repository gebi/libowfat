#include "scan.h"

size_t scan_int(const char* src,int* dest) {
  long l;
  size_t len=scan_long(src,&l);
  if (len) *dest=l;
  return len;
}
