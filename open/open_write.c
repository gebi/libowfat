#include <unistd.h>
#include <sys/fcntl.h>
#include "open.h"

extern int open_write(const char *filename) {
  return open(filename,O_WRONLY|O_CREAT|O_NDELAY,0644);
}
