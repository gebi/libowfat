#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include "io_internal.h"

#ifdef __MINGW32__
#include <stdio.h>
#endif

int64 io_canread() {
  io_entry* e;
  if (first_readable==-1)
#ifdef HAVE_SIGIO
  {
    if (alt_firstread>=0 && (e=array_get(&io_fds,sizeof(io_entry),alt_firstread)) && e->canread) {
      debug_printf(("io_canread: normal read queue is empty, swapping in alt read queue (starting with %ld)\n",alt_firstread));
      first_readable=alt_firstread;
      alt_firstread=-1;
    } else
      return -1;
  }
#else
    return -1;
#endif
  for (;;) {
    int64 r;
    e=array_get(&io_fds,sizeof(io_entry),first_readable);
    if (!e) break;
    r=first_readable;
    first_readable=e->next_read;
    e->next_read=-1;
    debug_printf(("io_canread: dequeue %lld from normal read queue (next is %ld)\n",r,first_readable));

#ifdef __MINGW32__
//    printf("event on %d: wr %d rq %d aq %d\n",(int)r,e->wantread,e->readqueued,e->acceptqueued);
#endif

    if (e->wantread &&
#ifdef __MINGW32__
		       (e->canread || e->acceptqueued==1 || e->readqueued==1)
#else
			e->canread
#endif
				  ) {
#ifdef HAVE_SIGIO
      e->next_read=alt_firstread;
      alt_firstread=r;
      debug_printf(("io_canread: enqueue %ld in alt read queue (next is %ld)\n",alt_firstread,e->next_read));
      if (io_waitmode!=_SIGIO)
#endif
	e->canread=0;
      return r;
    }
  }
  return -1;
}
