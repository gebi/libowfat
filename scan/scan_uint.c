#include "scan.h"

unsigned int scan_uint(const char* src,unsigned int* dest) {
  unsigned long l;
  register int len=scan_ulong(src,&l);
  *dest=l;
  return len;
}
