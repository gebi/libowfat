#include "scan.h"

size_t scan_asn1dertag(const char* src,size_t len,unsigned long long* length) {
  size_t n;
  unsigned long long l=0;
  for (n=0; n<len; ++n) {
    l=(l<<7) | (src[n]&0x7f);
    if (!(src[n]&0x80)) {
      *length=l;
      return n+1;
    }
  }
  return 0;
}
