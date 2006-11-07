#include <sys/types.h>
#include <unistd.h>
#ifdef __MINGW32__
#include <windows.h>
#else
#include <sys/mman.h>
#endif
#include "open.h"
#include "mmap.h"

int mmap_unmap(char* mapped,size_t maplen) {
#ifdef __MINGW32__
  (void)maplen;
  return UnmapViewOfFile(mapped)?0:-1;
#else
  return munmap(mapped,maplen);
#endif
}
