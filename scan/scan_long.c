#include "scan.h"

size_t scan_long(const char *src,long *dest) {
  register const char *tmp;
  register long int l;
  register unsigned char c;
  int neg;
  int ok;
  tmp=src; l=0; ok=neg=0;
  switch (*tmp) {
  case '-': neg=1;
  case '+': ++tmp;
  }
  while ((c=*tmp-'0')<10) {
    l=l*10+c;
    ++tmp;
    ok=1;
  }
  if (ok) *dest=(neg?-l:l);
  return tmp-src;
}
