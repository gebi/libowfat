#include "byte.h"

/* byte_rchr returns the largest integer i between 0 and len-1 inclusive
 * such that one[i] equals needle, or len if not found. */
unsigned int byte_rchr(const void* haystack,unsigned int len,char needle) {
  register char c=needle;
  register const char* s=haystack;
  register const char* t=s+len;
  for (;;) {
    --t; if (s>=t) break; if (*t==c) break;
    --t; if (s>=t) break; if (*t==c) break;
    --t; if (s>=t) break; if (*t==c) break;
    --t; if (s>=t) break; if (*t==c) break;
  }
  return t-s;
}
