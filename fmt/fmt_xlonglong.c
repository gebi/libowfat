#include "fmt.h"

unsigned int fmt_xlonglong(char *dest,unsigned long long i) {
  int tmp=0;
  if (i>>32) {
    tmp=fmt_xlong(dest,i>>32);
    if (dest) dest+=tmp;
  }
  return tmp+fmt_xlong(dest,i&0xffffffff);
}
