#include "fmt.h"

unsigned int fmt_strn(char *out,const char *in,unsigned int limit) {
  register char* s=out;
  register const char* t=in;
  register const char* u=out+limit;
  for (;;) {
    if (!*t) break; *s=*t; if (s==u) break; ++s; ++t;
    if (!*t) break; *s=*t; if (s==u) break; ++s; ++t;
    if (!*t) break; *s=*t; if (s==u) break; ++s; ++t;
    if (!*t) break; *s=*t; if (s==u) break; ++s; ++t;
  }
  return s-out;
}
