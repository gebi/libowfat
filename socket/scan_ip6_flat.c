#include "haveinline.h"

static inline int fromhex(unsigned char c) {
  if (c>='0' && c<='9')
    return c-'0';
  else if (c>='A' && c<='F')
    return c-'A'+10;
  else if (c>='a' && c<='f')
    return c-'a'+10;
  return -1;
}

unsigned int scan_ip6_flat(const char *s,char ip[16])
{
  int i;
  for (i=0; i<16; i++) {
    int tmp;
    tmp=fromhex(*s++);
    if (tmp<0) return 0;
    ip[i]=tmp << 4;
    tmp=fromhex(*s++);
    if (tmp<0) return 0;
    ip[i]+=tmp;
  }
  return 32;
}
