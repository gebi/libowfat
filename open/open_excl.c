#include <unistd.h>
#include <sys/fcntl.h>
#include "open.h"

extern int open_excl(const char *filename) {
  return open(filename,O_WRONLY|O_NDELAY|O_TRUNC|O_CREAT|O_EXCL,0600);
}
