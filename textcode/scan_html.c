#include "fmt.h"
#include "textcode.h"
#include "haveinline.h"
#include "case.h"

size_t scan_html(const char *src,char *dest,size_t *destlen) {
  register const unsigned char* s=(const unsigned char*) src;
  size_t written=0,i;
  for (i=0; s[i]; ++i) {
    if (s[i]=='&') {
      if (case_starts((const char*)s+i+1,"amp;")) {
	dest[written]='&';
	i+=4;
      } else if (case_starts((const char*)s+i+1,"lt;")) {
	dest[written]='<';
	i+=3;
      } else if (case_starts((const char*)s+i+1,"gt;")) {
	dest[written]='>';
	i+=3;
      }
    } else if (s[i]=='<') {
      if (case_starts((const char*)s+i+1,"br>")) {
	dest[written]='\n';
	i+=3;
      } else if (case_starts((const char*)s+i+1,"p>")) {
	dest[written]='\n'; ++written;
	dest[written]='\n';
	i+=3;
      }
    } else
      dest[written]=s[i];
    ++written;
  }
  *destlen=written;
  return i;
}
