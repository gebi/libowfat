#include "byte.h"
#include "errmsg.h"

int main() {
  char buf[4096];
  carp("both aligned");
  byte_copy(buf,16,"this is a test!\n");
  carp("destination aligned, source unaligned");
  byte_copy(buf,1000,buf+1);
  carp("destination unaligned, source aligned");
  byte_copy(buf+1,1000,buf);
  carp("both unaligned");
  byte_copy(buf+1,1000,buf+3);
  return 0;
}
