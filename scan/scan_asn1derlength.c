#include "scan.h"

size_t scan_asn1derlength(const char* src,size_t len,unsigned long long* length) {
  const char* orig=src;
  const char* max=orig+len;
  if (src>=max) return 0;
/* If the highest bit of the first byte is clear, the byte is the length.
 * Otherwise the next n bytes are the length (n being the lower 7 bits) */
  if (*src&0x80) {
    int chars=*src&0x7f;
    unsigned long long l=0;
    while (chars>0) {
      if (++src>=max) return 0;
      if (l>(((unsigned long long)-1)>>8)) return 0;	/* catch integer overflow */
      l=l*256+(unsigned char)*src;
      --chars;
    }
    *length=l;
  } else
    *length=*src&0x7f;
  src++;
  if (src+*length>max) return 0;	/* catch integer overflow */
  if ((uintptr_t)src+*length<(uintptr_t)src) return 0;	/* gcc 4.1 removes this check without the cast to uintptr_t */
  return src-orig;
}
