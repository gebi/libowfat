#include "stralloc.h"
#include "str.h"

extern int stralloc_copys(stralloc *sa,const unsigned char *buf) {
  return stralloc_copyb(sa,buf,str_len(buf));
}

