#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include "socket.h"
#include "haven2i.h"

#ifdef HAVE_N2I
static char ifname[IFNAMSIZ];

const char* socket_getifname(uint32 interface) {
  char *tmp=if_indextoname(interface,ifname);
  if (tmp)
    return tmp;
  else
    return "[unknown]";
}
#else
const char* socket_getifname(uint32 interface) {
  return "[unknown]";
}
#endif
