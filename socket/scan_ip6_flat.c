#include "scan.h"

unsigned int scan_ip6_flat(const char *s,char ip[16])
{
  int i;
  for (i=0; i<16; i++) {
    int tmp;
    tmp=scan_fromhex(*s++);
    if (tmp<0) return 0;
    ip[i]=tmp << 4;
    tmp=scan_fromhex(*s++);
    if (tmp<0) return 0;
    ip[i]+=tmp;
  }
  return 32;
}
