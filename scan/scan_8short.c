#include "scan.h"

unsigned int scan_8short(const char* src,unsigned short* dest) {
  unsigned long l;
  register int len=scan_8long(src,&l);
  *dest=l;
  return len;
}
