#include "scan.h"
#include <ctype.h>

unsigned int scan_noncharsetnskip(const char *s,const char *charset,unsigned int limit) {
  register const char *t=s;
  register const char *u=t+limit;
  register const char* i;
  while (t<u) {
    for (i=charset; *i; ++i)
      if (*i==*t) break;
    if (*i==*t) break;
    ++t;
  }
  return t-s;
}
