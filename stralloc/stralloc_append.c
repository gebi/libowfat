#include "stralloc.h"

/* stralloc_append adds one byte buf[0] to the end of the string stored
 * in sa. It is the same as stralloc_catb(&sa,buf,1). */
int stralloc_append(stralloc *sa,const char *in) {
  if (stralloc_readyplus(sa,1)) {
    sa->s[sa->len]=*in;
    ++sa->len;
    return 1;
  }
  return 0;
}

