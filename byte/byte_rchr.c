#include "byte.h"

/* byte_rchr returns the largest integer i between 0 and len-1 inclusive
 * such that one[i] equals needle, or len if not found. */
unsigned long byte_rchr(const void* haystack,unsigned long len,char needle) {
  register char c=needle;
  register const char* s=haystack;
  register const char* t=s+len;
  for (;;) {
    --t; if (s>t) break; if (*t==c) return t-s;
    --t; if (s>t) break; if (*t==c) return t-s;
    --t; if (s>t) break; if (*t==c) return t-s;
    --t; if (s>t) break; if (*t==c) return t-s;
  }
  return len;
}
