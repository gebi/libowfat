#include "stralloc.h"
#include "textcode.h"

int fmt_to_sa(unsigned long (*func)(char*,const char*,unsigned long),
	      stralloc* sa,const char* src,unsigned long len) {
  unsigned long needed=func(0,src,len);
  if (!stralloc_readyplus(sa,needed)) return 0;
  func(sa->s+sa->len,src,len);
  sa->len+=needed;
  return needed;
}
