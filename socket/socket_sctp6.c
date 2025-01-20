#include <unistd.h>
#include <libowfat/socket.h>
#include <libowfat/ndelay.h>

int socket_sctp6(void) {
  int s=socket_sctp6b();
  if (s==-1) return -1;
  if (ndelay_on(s) == -1) { close(s); return -1; }
  return s;
}
