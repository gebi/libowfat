#include "scan.h"

size_t scan_longlong(const char* src,signed long long* dest) {
  unsigned int i,o;
  unsigned long long l;
  char c=src[0];
  o=c=='-' || c=='+';
  if ((i=scan_ulonglong(src+o,&l))) {
    if (i+o) *dest=c=='-'?-l:l;
    return i+o;
  } return 0;
}
