#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "socket.h"

int socket_tcp6(void) {
  return socket(PF_INET6,SOCK_STREAM,IPPROTO_TCP);
}
