#include "str.h"

#ifdef __dietlibc__
unsigned int str_len(const char* in) __attribute__((alias("strlen")));
#else
unsigned int str_len(const char* in) {
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
