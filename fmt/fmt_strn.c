#include "fmt.h"

unsigned long fmt_strn(char *out,const char *in,unsigned long limit) {
  register char* s=out;
  register const char* t=in;
  register const char* u=in+limit;
  for (;;) {
    if (!*t) break; if (s) { *s=*t; ++s; } if (t==u) break; ++t;
    if (!*t) break; if (s) { *s=*t; ++s; } if (t==u) break; ++t;
    if (!*t) break; if (s) { *s=*t; ++s; } if (t==u) break; ++t;
    if (!*t) break; if (s) { *s=*t; ++s; } if (t==u) break; ++t;
  }
  return t-in;
}
