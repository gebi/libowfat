#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket.h"
#include "byte.h"
#include "haveip6.h"
#include "ip6.h"

#ifndef IPV6_MULTICAST_HOPS
#undef LIBC_HAS_IP6
#endif

int socket_mchopcount6(int s,char TTL)
{
#ifdef LIBC_HAS_IP6
  return setsockopt(s,IPPROTO_IPV6,IPV6_MULTICAST_HOPS,&TTL,sizeof TTL);
#else
  return socket_mcttl4(s,TTL);
#endif
}
