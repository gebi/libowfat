#include <sys/types.h>
#ifndef __MINGW32__
#include <sys/socket.h>
#endif
#include "socket.h"
#include "windoze.h"

int socket_bind4_reuse(int s,const char *ip,uint16 port) {
  int one=1;
  setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&one,sizeof one);
  return winsock2errno(socket_bind4(s,ip,port));
}
