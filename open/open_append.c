#include <unistd.h>
#include <sys/fcntl.h>
#include "open.h"

extern int open_append(const char *filename) {
  return open(filename,O_WRONLY|O_NDELAY|O_APPEND|O_CREAT,0600);
}
