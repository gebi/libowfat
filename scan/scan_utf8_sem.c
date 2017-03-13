#include "scan.h"

size_t scan_utf8_sem(const char* in,size_t len,uint32_t* num) {
  size_t r=scan_utf8(in,len,num);
  if (r>0) {
    if (*num>=0xd800 && *num<=0xdfff) return 0;
    if ((*num&0xfffe)==0xfffe) return 0;
    if (*num>=0xfdd0 && *num<=0xfdef) return 0;
  }
  return r;
}
