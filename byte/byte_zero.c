#include "byte.h"

/* byte_zero sets the bytes out[0], out[1], ..., out[len-1] to 0 */
void byte_zero(void* out, size_t len) {
  register char* s=out;
  register const char* t=s+len;
  for (;;) {
    if (s==t) break; *s=0; ++s;
    if (s==t) break; *s=0; ++s;
    if (s==t) break; *s=0; ++s;
    if (s==t) break; *s=0; ++s;
  }
}
