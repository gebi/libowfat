#include <sys/types.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/mman.h>
#ifndef INTERNAL
#include <libowfat/open.h>
#endif
#endif
#ifndef INTERNAL
#include <libowfat/mmap.h>
#endif

extern const char* mmap_read(const char* filename,size_t * filesize) {
#ifdef _WIN32
  HANDLE fd,m;
  char* map;
  fd=CreateFile(filename,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
  if (fd==INVALID_HANDLE_VALUE)
    return 0;
  m=CreateFileMapping(fd,0,PAGE_READONLY,0,0,NULL);
  map=0;
  if (m)
    if ((map=MapViewOfFile(m,FILE_MAP_READ,0,0,0)))
      *filesize=GetFileSize(fd,NULL);
  CloseHandle(m);
  CloseHandle(fd);
  return map;
#else
  int fd=open_read(filename);
  char *map;
  if (fd>=0) {
    off_t o=lseek(fd,0,SEEK_END);
    if (o==0 || (sizeof(off_t)!=sizeof(size_t) && o > (off_t)(size_t)-1)) { close(fd); return 0; }
    *filesize=(size_t)o;
    if (o>0) {
      map=(char*)mmap(0,*filesize,PROT_READ,MAP_SHARED,fd,0);
      if (map==(char*)-1)
	map=0;
    } else
      map=(char*)"";
    close(fd);
    return map;
  }
  return 0;
#endif
}
