#include "scan.h"
#include <ctype.h>

unsigned int scan_whitenskip(const char *s,unsigned int limit) {
  register const char *t=s;
  register const char *u=t+limit;
  while (t<u && isspace(*t)) ++t;
  return t-s;
}
