#include <buffer.h>
#include <unistd.h>

void buffer_close(buffer* b) {
  if (b->fd != -1) close(b->fd);
  if (b->deinit) b->deinit(b);
}
