#include "scan.h"

size_t scan_asn1derlengthvalue(const char* src,size_t len,unsigned long long* value) {
  if (len==0 || len>=-(uintptr_t)src) return 0;
  unsigned char i,c=*src;
  unsigned long long l;
  if ((c&0x80)==0) {
    *value=c&0x7f;
    return 1;
  }
  /* Highest bit set: lower 7 bits is the length of the length value in bytes. */
  c&=0x7f;
  if (!c) return 0;	/* length 0x80 means indefinite length encoding, not supported here */
  l=(unsigned char)src[1];
  if (l==0) return 0;	/* not minimally encoded: 0x81 0x00 instead of 0x00 */
  if (c>sizeof(l)) return 0;	/* too many bytes, does not fit into target integer type */
  for (i=2; i<=c; ++i)
    l=l*256+(unsigned char)src[i];
  if (l<0x7f) return 0;	/* not minimally encoded: 0x81 0x70 instead of 0x70 */
  *value=l;
  return i;
}

size_t scan_asn1derlength(const char* src,size_t len,unsigned long long* value) {
  unsigned long long l;
  size_t i=scan_asn1derlengthvalue(src,len,&l);
  if (l > len-i) return 0;		/* make sure data would fit into buffer */
  *value=l;
  return i;
}
