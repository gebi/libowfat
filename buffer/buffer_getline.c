#include <buffer.h>

int buffer_getline(buffer* b,char* x,unsigned long int len) {
  return buffer_get_token(b,x,len,"\n",1);
}
