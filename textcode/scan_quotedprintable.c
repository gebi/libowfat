#include "fmt.h"
#include "textcode.h"
#include "scan.h"

unsigned long scan_quotedprintable(const char *src,char *dest,unsigned long *destlen) {
  register const unsigned char* s=(const unsigned char*) src;
  unsigned long written=0,i;
  for (i=0; s[i]; ++i) {
    if (s[i]=='=') {
      int j=scan_fromhex(s[i+1]);
      if (j<0) break;
      dest[written]=j<<4;
      j=scan_fromhex(s[i+2]);
      if (j<0) break;
      dest[written]|=j;
      i+=2;
    } else {
      dest[written]=s[i];
    }
    ++written;
  }
  *destlen=written;
  return i;
}
