#include <sys/types.h>
#ifndef __MINGW32__
#include <sys/socket.h>
#endif
#include "socket.h"
#include "windoze.h"

int socket_bind6_reuse(int s,const char *ip,uint16 port,uint32 scope_id) {
  int one=1;
  setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&one,sizeof one);
  return winsock2errno(socket_bind6(s,ip,port,scope_id));
}
