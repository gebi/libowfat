#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "socket.h"

int socket_udp6(void) {
  return socket(PF_INET6,SOCK_DGRAM,IPPROTO_UDP);
}
