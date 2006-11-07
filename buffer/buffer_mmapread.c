#include <buffer.h>
#include <mmap.h>

static ssize_t op() {
  return 0;
}

int buffer_mmapread(buffer* b,const char* filename) {
  if (!(b->x=mmap_read(filename,&b->n))) return -1;
  b->p=0; b->a=b->n;
  b->fd=-1;
  b->op=op;
  b->todo=MUNMAP;
  return 0;
}
