#include <stdarg.h>
#include "stralloc.h"

int stralloc_catm_internal(stralloc* sa, ...) {
  va_list a;
  const char* s;
  va_start(a,sa);
  while ((s=va_arg(a,const char*)))
    if (stralloc_cats(sa,s)==0) {
      va_end(a);
      return 0;
    }
  va_end(a);
  return 1;
}
