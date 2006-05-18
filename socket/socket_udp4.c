#include <sys/types.h>
#ifndef __MINGW32__
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#include "windoze.h"

#include "socket.h"
#include "ndelay.h"

int socket_udp4(void) {
  int s;
  __winsock_init();
  s = winsock2errno(socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP));
  if (s == -1) return -1;
  if (ndelay_on(s) == -1) { close(s); return -1; }
  return s;
}
