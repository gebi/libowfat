#include "byte.h"

/* byte_copy copies in[0] to out[0], in[1] to out[1], ... and in[len-1]
 * to out[len-1]. */
void byte_copy(void* out, unsigned int len, const void* in) {
  register char* s=out;
  register const char* t=in;
  register const char* u=in+len;
  for (;;) {
    if (t==u) break; *s=*t; ++s; ++t;
    if (t==u) break; *s=*t; ++s; ++t;
    if (t==u) break; *s=*t; ++s; ++t;
    if (t==u) break; *s=*t; ++s; ++t;
  }
}
