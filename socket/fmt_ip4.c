#include "fmt.h"
#include "ip4.h"

unsigned int fmt_ip4(char *s,const char ip[4])
{
  unsigned int len;
  int i;

  len = 0;
  for (i=0; i<4; ++i) {
    register unsigned int j;
    len+=(j=fmt_ulong(s,(unsigned long) (unsigned char) ip[i]))+1;
    if (s && i<3) { s+=j; *s++='.'; }
  }
  return len-1;
}
