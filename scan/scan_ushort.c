#include "scan.h"

unsigned int scan_ushort(const char* src,unsigned short* dest) {
  unsigned long l;
  register int len=scan_ulong(src,&l);
  *dest=l;
  return len;
}
