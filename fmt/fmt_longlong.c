#include "fmt.h"

unsigned int fmt_longlong(char *dest,signed long long int i) {
  if (i<0) {
    if (dest) *dest++='-';
    return fmt_ulonglong(dest,-i)+1;
  } else
    return fmt_ulonglong(dest,i);
}
