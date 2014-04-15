#include "scan.h"

size_t scan_asn1dertag(const char* src,size_t len,unsigned long long* length) {
  size_t n;
  unsigned long long l;
  unsigned int bits=0;
  if (len==0) return 0;
  {
    unsigned int k=src[0]&0x7f;
    if (!(src[0]&0x80)) {
      *length=k;
      return 1;
    }
    if (!(l=k)) return 0;	// non-minimal encoding
    while (k) {	// count bits in leading byte
      ++bits;
      k>>=1;
    }
    bits=sizeof(l)*8-bits;
  }
  for (n=1; n<len; ++n) {
    if (bits<7) return 0;
    bits-=7;
    l=(l<<7) | (src[n]&0x7f);
    if (!(src[n]&0x80)) {
      *length=l;
      return n+1;
    }
  }
  return 0;
}
