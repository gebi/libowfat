#include "str.h"
#include "stralloc.h"
#include "textcode.h"

unsigned long scan_to_sa(unsigned long (*func)(const char*,char*,unsigned long*),
			 const char* src,stralloc* sa) {
  unsigned long scanned;
  unsigned long r;
  if (!stralloc_readyplus(sa,str_len(src))) return 0;
  if ((r=func(src,sa->s+sa->len,&scanned)))
    sa->len+=r;
  return r;
}
