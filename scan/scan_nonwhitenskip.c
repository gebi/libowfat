#include "scan.h"
#include <ctype.h>

unsigned long scan_nonwhitenskip(const char *s,unsigned long limit) {
  register const char *t=s;
  register const char *u=t+limit;
  while (t<u && !isspace(*t)) ++t;
  return t-s;
}
