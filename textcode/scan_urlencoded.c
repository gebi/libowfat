#include "fmt.h"
#include "textcode.h"
#include "haveinline.h"

static inline int fromhex(char c) {
  if (c>='0' && c<='9') return c-'0';
  if (c>='A' && c<='F') return c-'A'+10;
  if (c>='a' && c<='f') return c-'a'+10;
  return -1;
}

unsigned int scan_urlencoded(const char *src,char *dest,unsigned int *destlen) {
  register const unsigned char* s=(const unsigned char*) src;
  unsigned long written=0,i;
  for (i=0; s[i]; ++i) {
    if (s[i]=='%') {
      int j=fromhex(s[i+1]);
      if (j<0) break;
      dest[written]=j<<4;
      j=fromhex(s[i+2]);
      if (j<0) break;
      dest[written]|=j;
      i+=2;
    } if (s[i]=='+')
      dest[written]=' ';
    else
      dest[written]=s[i];
    ++written;
  }
  *destlen=written;
  return i;
}
