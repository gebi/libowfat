#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket.h"

int socket_mcloop4(int s,char loop)
{
  return setsockopt(s,IPPROTO_IP,IP_MULTICAST_LOOP,&loop,sizeof loop);
}

