#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "byte.h"
#include "socket.h"

int socket_local4(int s,char ip[4],uint16 *port)
{
  struct sockaddr_in si;
  unsigned int len = sizeof si;

  if (getsockname(s,(struct sockaddr *) &si,&len) == -1) return -1;
  *(uint32*)ip = *(uint32*)&si.sin_addr;
  uint16_unpack_big((char *) &si.sin_port,port);
  return 0;
}

