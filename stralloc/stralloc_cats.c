#include "stralloc.h"
#include "str.h"

extern int stralloc_cats(stralloc *sa,const unsigned char *buf) {
  return stralloc_catb(sa,buf,str_len(buf));
}

