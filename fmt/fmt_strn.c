#include "fmt.h"

size_t fmt_strn(char *out,const char *in,size_t limit) {
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
