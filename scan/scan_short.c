#include "scan.h"

unsigned int scan_short(const char* src,short* dest) {
  long l;
  register int len=scan_long(src,&l);
  *dest=l;
  return len;
}
