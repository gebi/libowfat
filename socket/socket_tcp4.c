#include <sys/types.h>
#ifndef __MINGW32__
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#include "windoze.h"
#include "socket.h"

int socket_tcp4(void) {
  __winsock_init();
  return winsock2errno(socket(PF_INET,SOCK_STREAM,IPPROTO_TCP));
}
