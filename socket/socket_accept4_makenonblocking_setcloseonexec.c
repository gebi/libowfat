#ifndef __MINGW32__
#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/socket.h>
#endif
#include <string.h>
#include "windoze.h"
#include "socket.h"

int socket_accept4_makenonblocking(int s,char *ip,uint16 *port) {
  return socket_accept4_flags(s,ip,port,SOCK_NONBLOCK | SOCK_CLOEXEC);
}
