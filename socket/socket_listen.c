#include <sys/types.h>
#ifndef __MINGW32__
#include <sys/socket.h>
#endif
#include "socket.h"
#include "windoze.h"

int socket_listen(int s,unsigned int backlog) {
  return winsock2errno(listen(s, backlog));
}
