#include <sys/types.h>
#include <sys/socket.h>

int socket_listen(int s,unsigned int backlog) {
  return listen(s, backlog);
}
