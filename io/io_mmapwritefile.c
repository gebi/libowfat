#include <io_internal.h>
#include <iob.h>
#include <unistd.h>
#include <sys/types.h>
#ifdef __MINGW32__
#include <windows.h>
#else
#include <sys/mman.h>
#endif
#include <errno.h>

#define BUFSIZE 16384

int64 io_mmapwritefile(int64 out,int64 in,uint64 off,uint64 bytes,io_write_callback writecb) {
  char buf[BUFSIZE];
  int n,m;
  uint64 sent=0;
  io_entry* e=array_get(&io_fds,sizeof(io_entry),out);
  if (e) {
    const char* c;
    unsigned long left;
#ifdef __MINGW32__
    if (!e->mh) e->mh=CreateFileMapping(out,0,PAGE_READONLY,0,0,NULL);
    if (!e->mh) goto readwrite;
#endif
    do {
      if (e->mmapped) {
	/* did we already map the right chunk? */
	if (off>=e->mapofs && off<e->mapofs+e->maplen)
	  goto mapok;	/* ok; mmapped the right chunk*/
#ifdef __MINGW32__
	UnmapViewOfFile(e->mmapped);
#else
	munmap(e->mmapped,e->maplen);
#endif
      }
      e->mapofs=off&0xffffffffffff0000ull;
      if (e->mapofs+0x10000>off+bytes)
	e->maplen=off+bytes-e->mapofs;
      else
	e->maplen=0x10000;
#ifdef __MINGW32__
      if ((e->mmapped=MapViewOfFile(e->mh,FILE_MAP_READ,(DWORD)(e->mapofs>>32),
				    (DWORD)e->mapofs,e->maplen))==0)
#else
      if ((e->mmapped=mmap(0,e->maplen,PROT_READ,MAP_SHARED,in,e->mapofs))==MAP_FAILED)
#endif
      {
	e->mmapped=0;
	goto readwrite;
      }
  mapok:
      c=(const char*)(e->mmapped)+(off&0xffff);
      left=e->maplen-(off&0xffff);
      if (left>bytes) left=bytes;
      while (left>0) {
	m=writecb(out,c,left);
	if (m==-1) {
	  e->canwrite=0;
	  e->next_write=-1;
	  if (errno!=EAGAIN) {
#ifdef __MINGW32__
	    UnmapViewOfFile(e->mmapped);
#else
	    munmap(e->mmapped,e->maplen);
#endif
	    e->mmapped=0;
	    return -3;
	  }
	  return sent?(int64)sent:-1;
	}
	if (m==0) return sent;
	sent+=m;
	left-=m;
	bytes-=m;
	off+=m;
	c+=m;
      }
    } while (bytes);
    if (e->mmapped) {
#ifdef __MINGW32__
      UnmapViewOfFile(e->mmapped);
#else
      munmap(e->mmapped,e->maplen);
#endif
      e->mmapped=0;
    }
    return sent;
  }
readwrite:
  if (lseek(in,off,SEEK_SET) != (off_t)off)
    return -1;
  while (bytes>0) {
    char* tmp=buf;
    if ((n=read(in,tmp,(bytes<BUFSIZE)?bytes:BUFSIZE))<=0)
      return (sent?(int64)sent:-1);
    while (n>0) {
      if ((m=writecb(out,tmp,n))<0) {
	if (m==-1) {
	  if (e) {
	    e->canwrite=0;
	    e->next_write=-1;
	  }
	  return errno==EAGAIN?(sent?(int64)sent:-1):-3;
	}
	goto abort;
      }
      sent+=m;
      n-=m;
      bytes-=m;
      tmp+=m;
    }
  }
abort:
  return sent;
}
