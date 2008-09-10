#include <unistd.h>
#include "socket.h"
#include "ndelay.h"

int socket_tcp4(void) {
  int s=socket_tcp4b();
  if (s==-1) return -1;
  if (ndelay_on(s) == -1) { close(s); return -1; }
  return s;
}
