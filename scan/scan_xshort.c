#include "scan.h"

unsigned int scan_xshort(const char* src,unsigned short* dest) {
  unsigned long l;
  register int len=scan_xlong(src,&l);
  *dest=l;
  return len;
}
