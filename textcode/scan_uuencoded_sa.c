#include "stralloc.h"
#include "textcode.h"

int scan_uuencoded_sa(const char *src,stralloc *sa) {
  unsigned int len;
  unsigned long tmp;
  register const unsigned char* s=(const unsigned char*) src;
  if ((len=*s-' ')>64) return 0; len&=63;
  ++s;
  while (len>0) {
    char dest[3];
    unsigned int l=len;
    if (s[0]-' '>64 || s[1]-' '>64 || s[2]-' '>64 || s[3]-' '>64) return 0;
    tmp=(((s[0]-' ')&077) << (3*6)) +
        (((s[1]-' ')&077) << (2*6)) +
        (((s[2]-' ')&077) << (1*6)) +
        (((s[3]-' ')&077));
    s+=4;
    if (len) { dest[0]=tmp>>16; --len; }
    if (len) { dest[1]=tmp>>8; --len; }
    if (len) { dest[2]=tmp&0xff; --len; }
    if (!stralloc_catb(sa,dest,l-len)) return 0;
  }
  return 1;
}
