#ifndef IO_H
#define IO_H

/* http://cr.yp.to/lib/io.html */

#include "uint64.h"
#include "taia.h"

/* like open(s,O_RDONLY) */
int io_readfile(int64* d,const char* s);
/* like open(s,O_WRONLY|O_TRUNC,0600) */
int io_createfile(int64* d,const char* s);
/* like pipe(d) */
int io_pipe(int64* d);

/* non-blocking read(), -1 for EAGAIN and -3+errno for other errors */
int64 io_tryread(int64 d,char* buf,int64 len);

/* blocking read(), with -3 instead of -1 for errors */
int64 io_waitread(int64 d,char* buf,int64 len);

/* non-blocking write(), -1 for EAGAIN and -3+errno for other errors */
int64 io_trywrite(int64 d,const char* buf,int64 len);

/* blocking write(), with -3 instead of -1 for errors */
int64 io_waitwrite(int64 d,const char* buf,int64 len);

/* modify timeout attribute of file descriptor */
void io_timeout(int64 d,tai6464 t);

/* like io_tryread but will return -2,errno=ETIMEDOUT if d has a timeout
 * associated and it is passed without input being there */
int64 io_tryreadtimeout(int64 d,char* buf,int64 len);

/* like io_trywrite but will return -2,errno=ETIMEDOUT if d has a timeout
 * associated and it is passed without being able to write */
int64 io_trywritetimeout(int64 d,const char* buf,int64 len);

void io_wantread(int64 d);
void io_wantwrite(int64 d);
void io_dontwantread(int64 d);
void io_dontwantwrite(int64 d);

void io_wait();
void io_waituntil(tai6464 t);
void io_check();

/* return next descriptor from io_wait that can be read from */
int64 io_canread();
/* return next descriptor from io_wait that can be written to */
int64 io_canwrite();

/* return next descriptor with expired timeout */
int64 io_timeouted();

/* put d on internal data structure, return 1 on success, 0 on error */
int io_fd(int64 d);

/* put descriptor in non-blocking mode */
void io_nonblock(int64 d);
/* put descriptor in close-on-exec mode */
void io_closeonexec(int64 d);

void io_close(int64 d);

#endif
