#include "fmt.h"

unsigned int fmt_str(char *out,const char *in) {
  register char* s=out;
  register const char* t=in;
  for (;;) {
    if (!*t) break; *s=*t; ++s; ++t;
    if (!*t) break; *s=*t; ++s; ++t;
    if (!*t) break; *s=*t; ++s; ++t;
    if (!*t) break; *s=*t; ++s; ++t;
  }
  return s-out;
}
