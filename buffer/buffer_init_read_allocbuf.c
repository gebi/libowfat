#include <unistd.h>
#include <libowfat/buffer.h>

int buffer_init_read_allocbuf(buffer* b, int fd, size_t ylen) {
  return buffer_init_allocbuf(b, read, fd, ylen);
}

