#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket.h"

int socket_mcttl4(int s,char TTL)
{
  return setsockopt(s,IPPROTO_IP,IP_MULTICAST_TTL,&TTL,sizeof TTL);
}

