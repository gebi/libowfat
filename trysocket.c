#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

main() {
  int fd=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
}
