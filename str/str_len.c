#include "str.h"

#ifdef __dietlibc__
#undef str_len
unsigned long str_len(const char* in) __attribute__((alias("strlen")));
#else
unsigned long str_len(const char* in) {
  register const char* t=in;
  for (;;) {
    if (!*t) break; ++t;
    if (!*t) break; ++t;
    if (!*t) break; ++t;
    if (!*t) break; ++t;
  }
  return t-in;
}
#endif
