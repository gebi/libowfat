#include <iob_internal.h>

int64 iob_write(int64 s,io_batch* b,io_write_callback cb) {
  iob_entry* e,* last;
  int64 total,sent;
  long i;
  int thatsit;

  if (b->bytesleft==0) return 0;
  last=(iob_entry*)(((char*)array_start(&b->b))+array_bytes(&b->b));
  total=0;
  if (!(e=array_get(&b->b,sizeof(iob_entry),b->next)))
    return -3;		/* can't happen error */
  thatsit=0;
  for (i=0; e+i<last; ++i) {
    if (!e[i].n) continue;
    if (e[i].type==FROMFILE  || e[i].type==FROMFILE_CLOSE)
      sent=io_mmapwritefile(s,e[i].fd,e[i].offset,e[i].n,cb);
    else
      sent=cb(s,e[i].buf+e[i].offset,e[i].n);
    if (sent>0 && sent>e[i].n) sent=e[i].n; /* can't happen */
    thatsit=(sent != e[i].n);
    if (sent<=0)
      return total?total:sent;
    e[i].offset+=sent;
    e[i].n-=sent;
    total+=sent;
    b->bytesleft-=sent;
    if (thatsit) break;
  }
  if (total == b->bytesleft)
    iob_reset(b);
  return total;
}
