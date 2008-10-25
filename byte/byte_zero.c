#include "byte.h"

/* byte_zero sets the bytes out[0], out[1], ..., out[len-1] to 0 */
void byte_zero(void* out, size_t len) {
#if 1
  /* gcc 4.3.1 generates wrong code for this, so I'm switching to
   * simpler code */
  register char* s=out;
  size_t i;
  for (i=0; i<len; ++i)
    s[i]=0;
#else
  register char* s=out;
  register const char* t=s+len;
  for (;;) {
    if (s==t) break; *s=0; ++s;
    if (s==t) break; *s=0; ++s;
    if (s==t) break; *s=0; ++s;
    if (s==t) break; *s=0; ++s;
  }
#endif
}
