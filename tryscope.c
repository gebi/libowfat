#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

main() {
  struct sockaddr_in6 sa;
  sa.sin6_family = PF_INET6;
  sa.sin6_scope_id = 23;
}
