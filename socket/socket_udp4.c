#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "socket.h"

int socket_udp4(void) {
  return socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
}
