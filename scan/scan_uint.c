#include "scan.h"

/* this is cut and paste from scan_ulong instead of calling scan_ulong
 * because this way scan_uint can abort scanning when the number would
 * not fit into an unsigned int (as opposed to not fitting in an
 * unsigned long) */

size_t scan_uint(const char* src,unsigned int* dest) {
  if (sizeof(unsigned int) == sizeof(unsigned long)) {
    /* a good optimizing compiler should remove the else clause when not
     * needed */
    return scan_ulong(src,(unsigned long*)dest);
  } else {
    register const char *tmp=src;
    register unsigned int l=0;
    register unsigned char c;
    while ((c=*tmp-'0')<10) {
      unsigned int n;
      /* division is very slow on most architectures */
      n=l<<3; if ((n>>3)!=l) break;
      if (n+(l<<1) < n) break;
      n+=l<<1;
      if (n+c < n) break;
      l=n+c;
      ++tmp;
    }
    if (tmp-src) *dest=l;
    return tmp-src;
  }
}
