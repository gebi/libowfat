#include "ip6.h"
#include "haveinline.h"

static inline char tohex(char c) {
  return c>=10?c-10+'a':c+'0';
}

unsigned int fmt_ip6_flat(char *s,const char ip[16])
{
  int i;
  if (!s) return 32;
  for (i=0; i<16; i++) {
    *s++=tohex((unsigned char)ip[i] >> 4);
    *s++=tohex((unsigned char)ip[i] & 15);
  }
  return 32;
}
