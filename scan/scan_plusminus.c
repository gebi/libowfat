#include "scan.h"

unsigned int scan_plusminus(const char *src,signed int *dest) {
  *dest=1;
  switch (*src) {
  case '-': *dest=-1;
  case '+': return 1; break;
  }
  return 0;
}
