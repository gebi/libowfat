#include "fmt.h"
#include "stralloc.h"
#include "textcode.h"

int fmt_yenc_sa(stralloc *sa,const char* src,unsigned int len) {
  register const unsigned char* s=(const unsigned char*) src;
  unsigned long i;
  int linelen=0;
  for (i=0; i<len; ++i) {
    unsigned char c=s[i]+42;
    switch (c) {
    case ' ':	/* escape space at line ending */
      if (linelen==253) {
        linelen=0;
        if (!stralloc_catb(sa,"\n",1)) return 0;
      }
      goto dontescape;
    case 'F':	/* escape "^From " */
      if (s[i+1]+42!='r' || s[i+2]+42!='o' || s[i+3]+42!='m' || s[i+4]+42!=' ') goto dontescape;
    case '.':	/* dot at start of line needs to be escaped */
      if (!linelen) goto dontescape;
      /* fall through */
    case 0:
    case '\n':
    case '\r':
    case '=':
      if (!stralloc_catb(sa,"=",1)) return 0;
      c+=64;
      /* fall through */
    default:
dontescape:
      ++linelen;
      if (!stralloc_catb(sa,&c,1)) return 0;
      if (linelen>253) {
	linelen=0;
        if (!stralloc_catb(sa,"\n",1)) return 0;
      }
    }
  }
  if (linelen) {
    linelen=0;
    if (!stralloc_catb(sa,"\n",1)) return 0;
  }
  return 1;
}
