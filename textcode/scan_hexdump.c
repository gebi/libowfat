#include "fmt.h"
#include "textcode.h"
#include "scan.h"

unsigned long scan_hexdump(const char *src,char *dest,unsigned long *destlen) {
  register const unsigned char* s=(const unsigned char*) src;
  unsigned long written=0,i;
  for (i=0; s[i]; ++i) {
    int j=scan_fromhex(s[i]);
    if (j<0) break;
    dest[written]=j<<4;
    j=scan_fromhex(s[i+1]);
    if (j<0) break;
    dest[written]|=j;
    ++i;
    ++written;
  }
  *destlen=written;
  return i;
}
