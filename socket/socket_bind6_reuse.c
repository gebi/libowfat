#include <sys/types.h>
#include <sys/socket.h>
#include "socket.h"

int socket_bind6_reuse(int s,const char *ip,uint16 port,uint32 scope_id) {
  int one=1;
  setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&one,sizeof one);
  return socket_bind6(s,ip,port,scope_id);
}
