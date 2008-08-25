#include "byte.h"

/* byte_copy copies in[0] to out[0], in[1] to out[1], ... and in[len-1]
 * to out[len-1]. */
void byte_copy(void* out, size_t len, const void* in) {
  char* s=out;
  const char* t=in;
#if 1
  /* gcc 4.3.1 generates wrong code for this, so I'm switching to
   * simpler code */
  size_t i;
  for (i=0; i<len; ++i)
    s[i]=t[i];
#else
  const char* u=t+len;
  if (len>127) {
    while ((unsigned long)s&(sizeof(unsigned long)-1)) {
      *s=*t; ++s; ++t;
    }
    /* s (destination) is now unsigned long aligned */
#ifndef __i386__
    if (!((unsigned long)t&(sizeof(unsigned long)-1)))
#endif
      while (t+sizeof(unsigned long)<=u) {
	*(unsigned long*)s=*(unsigned long*)t;
	s+=sizeof(unsigned long); t+=sizeof(unsigned long);
      }
  }
  for (;;) {
    if (t==u) break; *s=*t; ++s; ++t;
    if (t==u) break; *s=*t; ++s; ++t;
    if (t==u) break; *s=*t; ++s; ++t;
    if (t==u) break; *s=*t; ++s; ++t;
  }
#endif
}
