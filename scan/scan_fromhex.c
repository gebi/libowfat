#include "scan.h"

int scan_fromhex(unsigned char c) {
  if (c>='0' && c<='9')
    return c-'0';
  else if (c>='A' && c<='F')
    return c-'A'+10;
  else if (c>='a' && c<='f')
    return c-'a'+10;
  return -1;
}

