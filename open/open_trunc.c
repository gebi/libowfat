#include <unistd.h>
#include <sys/fcntl.h>
#include "open.h"

extern int open_trunc(const char *filename) {
  return open(filename,O_WRONLY|O_NDELAY|O_TRUNC|O_CREAT,0644);
}
