#include "scan.h"

unsigned int scan_int(const char* src,int* dest) {
  long l;
  register int len=scan_long(src,&l);
  *dest=l;
  return len;
}
