#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include "open.h"
#include "mmap.h"

extern char* mmap_read(const char* filename,unsigned long* filesize) {
  int fd=open_read(filename);
  char *map;
  if (fd>=0) {
    *filesize=lseek(fd,0,SEEK_END);
    map=mmap(0,*filesize,PROT_READ,MAP_SHARED,fd,0);
    if (map==(char*)-1)
      map=0;
    close(fd);
    return map;
  }
  return 0;
}
