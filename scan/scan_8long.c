#include "scan.h"

size_t scan_8long(const char *src,unsigned long *dest) {
  return scan_8longn(src,-1,dest);
}
