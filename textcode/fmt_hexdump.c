#include "fmt.h"
#include "textcode.h"
#include "str.h"
#include "haveinline.h"

unsigned long fmt_hexdump(char* dest,const char* src,unsigned long len) {
  register const unsigned char* s=(const unsigned char*) src;
  unsigned long written=0,i;
  for (i=0; i<len; ++i) {
    dest[written]=fmt_tohex(s[i]>>4);
    dest[written+1]=fmt_tohex(s[i]&15);
    written+=2;
  }
  return written;
}
