#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket.h"
#include "haveip6.h"

int socket_mcloop6(int s,char loop)
{
#ifdef LIBC_HAS_IP6
  if (noipv6)
    return setsockopt(s,IPPROTO_IP,IP_MULTICAST_LOOP,&loop,sizeof loop);
  else
    return setsockopt(s,IPPROTO_IPV6,IPV6_MULTICAST_LOOP,&loop,sizeof loop);
#else
  return setsockopt(s,IPPROTO_IP,IP_MULTICAST_LOOP,&loop,sizeof loop);
#endif
}

