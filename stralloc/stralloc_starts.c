#include "stralloc.h"
#include "byte.h"
#include "str.h"

extern int stralloc_starts(stralloc *sa,const char *in) {
  register unsigned long int len=str_len(in);
  return (len<=sa->len && !byte_diff(sa->s,len,in));
}

