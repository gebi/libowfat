#include "fmt.h"

unsigned int fmt_minus(char *dest,int i) {
  if (i<0) {
    if (dest) *dest='-';
    return 1;
  }
  return 0;
}
