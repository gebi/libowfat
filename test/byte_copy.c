#include "byte.h"
#include <write12.h>

int main() {
  char buf[4096];
  __write1("both aligned\n");
  byte_copy(buf,16,"this is a test!\n");
  __write1("destination aligned, source unaligned\n");
  byte_copy(buf,1000,buf+1);
  __write1("destination unaligned, source aligned\n");
  byte_copy(buf+1,1000,buf);
  __write1("both unaligned\n");
  byte_copy(buf+1,1000,buf+3);
}
