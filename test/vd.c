#include "socket.h"
#include "buffer.h"
#include <unistd.h>
#include <stdio.h>

main() {
  int s=socket_tcp4();
  char line[1024];
  char buf[4096];
  int l;
  int header=1;
  buffer filein;
  buffer_init(&filein,read,s,buf,sizeof buf);
  if (socket_connect4(s,"\x7f\x00\x00\x01",4000)) {
    perror("connect");
    return 1;
  }
  write(s,"vd\nq\n",5);
  for (;;) {
    line[0]=0;
    if ((l=buffer_getline(&filein,line,(sizeof line)-1))==0 && line[l]!='\n')
      break;
    else {
      line[l+1]=0;
      if (!header) {
	if (strcmp(line,"\e[7mMLdonkey command-line:\e[2;37;0m\n") &&
	    strcmp(line,"\e[2;37;0m\e[7mMLdonkey command-line:\e[2;37;0m\n") &&
	    strncmp(line,"> ",2))
	  buffer_put(buffer_1,line,l+1);
      }
      if (!strcmp(line,"Use \e[31m?\e[2;37;0m for help\n")) header=0;
      if (!strcmp(line,"Use ? for help\n")) header=0;
    }
  }
  buffer_flush(buffer_1);
}
