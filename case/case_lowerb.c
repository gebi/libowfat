#include "case.h"

void case_lowerb(void *S,unsigned long len) {
  char* s=(char*)S;
  unsigned char x;
  while (len > 0) {
    --len;
    x = *s - 'A';
    if (x <= 'Z' - 'A') *s = x + 'a';
    ++s;
  }
}
