#include <sys/types.h>
#include <sys/socket.h>
#include "socket.h"

int socket_bind4_reuse(int s,const char *ip,uint16 port) {
  int one=1;
  setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&one,sizeof one);
  return socket_bind4(s,ip,port);
}
