#include "fmt.h"

unsigned int fmt_plusminus(char *dest,int i) {
  if (i) {
    if (dest) *dest=(i>=0?'+':'-');
    return 1;
  }
  return 0;
}
