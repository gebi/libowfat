#include "io_internal.h"
#include <sys/types.h>
#include <signal.h>

void io_sigpipe(void) {
  static int isitdone;
  if (!isitdone) {
    signal(SIGPIPE,SIG_IGN);
    isitdone=1;
  }
}
