#include "scan.h"

unsigned int scan_ulonglong(const char *src,unsigned long long *dest) {
  register const char *tmp=src;
  register unsigned long long l=0;
  register unsigned char c;
  while ((c=*tmp-'0')<10) {
    unsigned long long m=l;
    l=l*10+c;
    if ((l>>3) < m) break;
    ++tmp;
  }
  *dest=l;
  return tmp-src;
}
