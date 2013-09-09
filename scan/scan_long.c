#include "scan.h"

size_t scan_long(const char *src,long *dest) {
  return scan_longn(src,-1,dest);
}
