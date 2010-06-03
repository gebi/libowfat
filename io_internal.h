#ifndef my_extern
#define my_extern extern
#endif

#include "io.h"
#include "array.h"
#ifdef __MINGW32__
#include "socket.h"
my_extern HANDLE io_comport;
#else
#include "haveepoll.h"
#include "havekqueue.h"
#include "havedevpoll.h"
#include "havesigio.h"
#ifdef HAVE_SIGIO
#define _GNU_SOURCE
#include <signal.h>
#endif
#endif

typedef struct {
  tai6464 timeout;
  unsigned int wantread:1;
  unsigned int wantwrite:1;
  unsigned int canread:1;
  unsigned int canwrite:1;
  unsigned int nonblock:1;
  unsigned int inuse:1;
#ifdef __MINGW32__
  unsigned int readqueued:2;
  unsigned int writequeued:2;
  unsigned int acceptqueued:2;
  unsigned int connectqueued:2;
  unsigned int sendfilequeued:2;
  unsigned int listened:1;
#endif
  long next_read;
  long next_write;
  void* cookie;
  void* mmapped;
  long maplen;
  uint64 mapofs;
#ifdef __MINGW32__
  OVERLAPPED or,ow,os;	/* overlapped for read+accept, write+connect, sendfile */
  HANDLE /* fd, */ mh;
  char inbuf[8192];
  int bytes_read,bytes_written;
  DWORD errorcode;
  SOCKET next_accept;
#endif
} io_entry;

extern int io_multithreaded;
extern int io_sockets[2];

my_extern array io_fds;
my_extern uint64 io_wanted_fds;
my_extern array io_pollfds;

my_extern long first_readable;
my_extern long first_writeable;

my_extern enum __io_waitmode {
  UNDECIDED,
  POLL
#ifdef HAVE_KQUEUE
  ,KQUEUE
#endif
#ifdef HAVE_EPOLL
  ,EPOLL
#endif
#ifdef HAVE_SIGIO
  ,_SIGIO
#endif
#ifdef HAVE_DEVPOLL
  ,DEVPOLL
#endif
#ifdef __MINGW32__
  ,COMPLETIONPORT
#endif
} io_waitmode;

#if defined(HAVE_KQUEUE) || defined(HAVE_EPOLL) || defined(HAVE_DEVPOLL)
my_extern int io_master;
#endif
#if defined(HAVE_SIGIO)
my_extern int io_signum;
my_extern sigset_t io_ss;

my_extern long alt_firstread;
my_extern long alt_firstwrite;
#endif

int64 io_waituntil2(int64 milliseconds);

void io_sigpipe(void);

/* return next descriptor from io_wait that can be read from */
int64 io_canread_unlocked();
/* return next descriptor from io_wait that can be written to */
int64 io_canwrite_unlocked();
/* return next descriptor with expired timeout */
int64 io_timeouted_unlocked();

struct eventpacket {
  int fd;
  enum { CANREAD, CANWRITE, TIMEOUT } what;
};

#define debug_printf(x)
