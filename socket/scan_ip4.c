#include "scan.h"
#include "ip4.h"

unsigned int scan_ip4(const char *s,char ip[4])
{
  unsigned int len;
  unsigned long u;
  int i;

  len = 0;
  for (i=0; i<4; ++i) {
    register unsigned int j;
    len+=(j=scan_ulong(s,&u))+1;
    if (!j) return 0;
    ip[i]=u; s+=j;
    if (i<3 && *s!='.') return 0; ++s;
  }
  return len-1;
}
