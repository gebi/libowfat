#include "fmt.h"
#include "textcode.h"

unsigned long scan_yenc(const char *src,char *dest,unsigned long *destlen) {
  register const unsigned char* s=(const unsigned char*) src;
  unsigned long written=0,i;
  for (i=0; s[i]; ++i) {
    if (s[i]=='=') {
      ++i;
      if (s[i]=='y') break;
      dest[written]=s[i]-64-42;
    } else if (s[i]=='\n' || s[i]=='\r' || s[i]=='\0')
      break;
    else
      dest[written]=s[i]-42;
    ++written;
  }
  *destlen=written;
  return i;
}
