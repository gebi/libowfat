#ifndef _IOB_H
#define _IOB_H

#include "io.h"

typedef struct io_batch io_batch;

io_batch* iob_new(int hint_entries);
int iob_addbuf(io_batch* b,void* buf,uint64 n);
int iob_addfile(io_batch* b,int64 fd,uint64 off,uint64 n);
int64 iob_send(int64 s,io_batch* b);
void iob_reset(io_batch* b);

#endif
