#include <unistd.h>
#include <sys/time.h>
#include <poll.h>
#include <errno.h>
#include "io_internal.h"
#include "safemult.h"

void io_waituntil(tai6464 t) {
  uint64 x,y;
  tai6464 now;
  taia_now(&now);
  if (!umult64(now.sec.x,1000,&x) || (y=x+now.nano/10000000)<x)
    y=-1;	/* overflow; wait indefinitely */
  io_waituntil2(y);
}
