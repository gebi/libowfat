#include "fmt.h"

unsigned int fmt_long(char *dest,long int i) {
  if (i<0) {
    if (dest) *dest='-';
    return fmt_ulong(dest+1,-i);
  } else
    return fmt_ulong(dest,i);
}
