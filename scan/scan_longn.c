#include "scan.h"

size_t scan_longn(const char *src,size_t n,long *dest) {
  register const char *tmp;
  register long int l;
  register unsigned char c;
  int neg;
  int ok;
  if (!n--) return 0;
  tmp=src; l=0; ok=neg=0;
  switch (*tmp) {
  case '-': neg=1;
  case '+': ++tmp;
  }
  while (n-->0 && (c=*tmp-'0')<10) {
    l=l*10+c;
    ++tmp;
    ok=1;
  }
  if (!ok) return 0;
  *dest=(neg?-l:l);
  return tmp-src;
}
