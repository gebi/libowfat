#include "stralloc.h"
#include <stdlib.h>

/* stralloc_readyplus is like stralloc_ready except that, if sa is
 * already allocated, stralloc_readyplus adds the current length of sa
 * to len. */
int stralloc_readyplus(stralloc *sa,unsigned int len) {
  if (sa->s)
    return stralloc_ready(sa,sa->len+len);
  else
    return stralloc_ready(sa,len);
}
