#include <buffer.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

void buffer_close(buffer* b) {
  if (b->fd != -1) close(b->fd);
  switch (b->todo) {
  case FREE: free(b->x); break;
  case MUNMAP: munmap(b->x,b->a); break;
  default: ;
  }
}
