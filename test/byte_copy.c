#include "byte.h"
#include "errmsg.h"
#include <string.h>

char buf[4096];
char text[128];

int main() {
  memset(buf,0,sizeof(buf));
  strcpy(text,"this is a test!\n");

//  carp("both aligned");
  byte_copy(buf,16,text);
  if (memcmp(buf,"this is a test!\n\0",18))
    die(1,"fail 1");

  memset(buf,0,sizeof(buf));
//  carp("destination aligned, source unaligned");
  byte_copy(buf,15,text+1);
  if (memcmp(buf,"his is a test!\n\0\0",18))
    die(1,"fail 2");

  memset(buf,0,sizeof(buf));
//  carp("destination unaligned, source aligned");
  byte_copy(buf+1,15,text);
  if (memcmp(buf,"\0this is a test!\0\0",18))
    die(1,"fail 3");

  memset(buf,0,sizeof(buf));
//  carp("both unaligned");
  byte_copy(buf+1,10,text+3);
  if (memcmp(buf,"\0s is a tes\0\0",14))
    die(1,"fail 4");

  return 0;
}
