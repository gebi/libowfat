#include "byte.h"

/* byte_chr returns the smallest integer i between 0 and len-1
 * inclusive such that one[i] equals needle, or len it not found. */
unsigned int byte_chr(const void* haystack, unsigned int len, char needle) {
  register char c=needle;
  register const char* s=haystack;
  register const char* t=s+len;
  for (;;) {
    if (s==t) break; if (*s==c) break; ++s;
    if (s==t) break; if (*s==c) break; ++s;
    if (s==t) break; if (*s==c) break; ++s;
    if (s==t) break; if (*s==c) break; ++s;
  }
  return s-(const char*)haystack;
}
