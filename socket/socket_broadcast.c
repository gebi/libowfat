#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket.h"

int socket_broadcast(int s)
{
  int opt = 1;
  return setsockopt(s,SOL_SOCKET,SO_BROADCAST,&opt,sizeof opt);
}

