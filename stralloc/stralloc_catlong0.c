#include "stralloc.h"
#include "fmt.h"

int stralloc_catlong0(stralloc *sa,signed long int in,size_t n) {
  if (stralloc_readyplus(sa,fmt_minus(0,in)+fmt_ulong0(0,in,n))) {
    sa->len+=fmt_minus(sa->s+sa->len,in);
    sa->len+=fmt_ulong0(sa->s+sa->len,in>=0?in:-in,n);
    return 1;
  } else
    return 0;
}
