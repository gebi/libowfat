#include "fmt.h"
#include "stralloc.h"
#include "textcode.h"

int scan_yenc_sa(const char *src,stralloc *sa) {
  register const unsigned char* s=(const unsigned char*) src;
  unsigned long i;
  for (i=0; s[i]; ++i) {
    char dest;
    if (s[i]=='=') {
      ++i;
      if (s[i]=='y') break;
      dest=s[i]-64-42;
    } else if (s[i]=='\n' || s[i]=='\r' || s[i]=='\0')
      break;
    else
      dest=s[i]-42;
    if (!stralloc_catb(sa,&dest,1)) return 0;
  }
  return 1;
}
