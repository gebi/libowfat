#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "socket.h"

int socket_tcp4(void) {
  return socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
}
