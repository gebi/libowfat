#include <assert.h>
#include "iob.h"
#include "buffer.h"

main() {
  int64 fd;
  io_batch* b;
  assert(io_readfile(&fd,"GNUmakefile"));
  assert(b=iob_new(10));
  assert(iob_addbuf(b,"Huhu",4));
  assert(iob_addbuf(b," fnord\n",7));
  assert(iob_addfile(b,fd,10,10));
  buffer_puts(buffer_2,"iob_send returned ");
  buffer_putlong(buffer_2,iob_send(1,b));
  buffer_putsflush(buffer_2,".\n");
}
