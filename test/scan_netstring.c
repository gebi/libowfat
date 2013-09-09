#include "scan.h"
#include <assert.h>

int main() {
  char* s;
  size_t l;
  const char* orig;
  orig="3:foo,"; assert(scan_netstring(orig,6,&s,&l)==6); assert(s==orig+2);
  orig="4294967295:foo,"; assert(scan_netstring(orig,15,&s,&l)==0);
  orig="18446744073709551615:foo,"; assert(scan_netstring(orig,25,&s,&l)==0);
}
