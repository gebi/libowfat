#include "scan.h"

unsigned int scan_ulong(const char* src,unsigned long int* dest) {
  register const char *tmp=src;
  register unsigned long int l=0;
  register unsigned char c;
  while ((c=*tmp-'0')<10) {
    unsigned long int m=l;
    l=l*10+c;
    if ((l>>3) < m) break;
    ++tmp;
  }
  *dest=l;
  return tmp-src;
}
