#include <unistd.h>
#include <sys/time.h>
#include <poll.h>
#include <errno.h>
#include "io_internal.h"
#include "safemult.h"

void io_waituntil(tai6464 t) {
  uint64 x,y;
  tai6464 now,diff;
  taia_now(&now);
  taia_sub(&diff,&t,&now);
  if (!umult64(diff.sec.x,1000,&x) || (y=x+diff.nano/10000000)<x)
    y=-1;	/* overflow; wait indefinitely */
  io_waituntil2(y);
}
