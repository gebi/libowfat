#include "scan.h"

size_t scan_varint(const char* in,size_t len, unsigned long long* n) {
  size_t i;
  unsigned long long l;
  if (len==0) return 0;
  l=0;
  for (l=0, i=0; i<len; ++i) {
    l+=(in[i]&0x7f) << (i*7);
    if (!(in[i]&0x80)) {
      *n=l;
      return i+1;
    }
  }
  return 0;
}

size_t scan_pb_type0_int(const char* dest,size_t len,unsigned long long* l) __attribute__((alias("scan_varint")));
