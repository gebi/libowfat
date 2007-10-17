#include "iob.h"
#include "array.h"

typedef struct iob_entry {
  enum { FROMBUF, FROMBUF_FREE, FROMBUF_MUNMAP, FROMFILE, FROMFILE_CLOSE } type;
  int64 fd;
  const char* buf;
  uint64 offset,n;
} iob_entry;

int iob_addbuf_internal(io_batch* b,const void* buf,uint64 n,int free);
