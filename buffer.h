#ifndef BUFFER_H
#define BUFFER_H

typedef struct buffer {
  char *x;        /* actual buffer space */
  unsigned int p; /* current position */
  unsigned int n; /* current size of string in buffer */
  unsigned int a; /* allocated buffer size */
  int fd;         /* passed as first argument to op */
  int (*op)();    /* use read(2) or write(2) */
} buffer;

#define BUFFER_INIT(op,fd,buf,len) { (buf), 0, 0, (len), (fd), (op) }
#define BUFFER_INIT_READ(op,fd,buf,len) BUFFER_INIT(op,fd,buf,len) /*obsolete*/
#define BUFFER_INSIZE 8192
#define BUFFER_OUTSIZE 8192

extern void buffer_init(buffer* b,int (*op)(),int fd,char* y,unsigned int ylen);

extern int buffer_flush(buffer* b);
extern int buffer_put(buffer* b,const char* x,unsigned int len);
extern int buffer_putalign(buffer* b,const char* x,unsigned int len);
extern int buffer_putflush(buffer* b,const char* x,unsigned int len);
extern int buffer_puts(buffer* b,const char* x);
extern int buffer_putsalign(buffer* b,const char* x);
extern int buffer_putsflush(buffer* b,const char* x);

extern int buffer_putspace(buffer* b);
extern int buffer_putnlflush(buffer* b); /* put \n and flush */

#define buffer_PUTC(s,c) \
  ( ((s)->a != (s)->p) \
    ? ( (s)->x[(s)->p++] = (c), 0 ) \
    : buffer_put((s),&(c),1) \
  )

extern int buffer_get(buffer* b,char* x,unsigned int len);
extern int buffer_feed(buffer* b);
extern int buffer_getc(buffer* b,char* x);
extern int buffer_getn(buffer* b,char* x,unsigned int len);

/* read bytes until the destination buffer is full (len bytes), end of
 * file is reached or the read char is in charset (setlen bytes).  An
 * empty line when looking for \n will write '\n' to x and return 0.  If
 * EOF is reached, \0 is written to the buffer */
extern int buffer_get_token(buffer* b,char* x,unsigned int len,const char* charset,unsigned int setlen);
#define buffer_getline(b,x,len) buffer_get_token((b),(x),(len),"\n",1)

/* this predicate is given the string as currently read from the buffer
 * and is supposed to return 1 if the token is complete, 0 if not. */
typedef int (*string_predicate)(const char* x,unsigned int len);

/* like buffer_get_token but the token ends when your predicate says so */
extern int buffer_get_token_pred(buffer* b,char* x,unsigned int len,string_predicate p);

extern char *buffer_peek(buffer* b);
extern void buffer_seek(buffer* b,unsigned int len);

#define buffer_PEEK(s) ( (s)->x + (s)->p )
#define buffer_SEEK(s,len) ( (s)->p += (len) )

#define buffer_GETC(s,c) \
  ( ((s)->p < (s)->n) \
    ? ( *(c) = *buffer_PEEK(s), buffer_SEEK((s),1), 1 ) \
    : buffer_get((s),(c),1) \
  )

extern int buffer_copy(buffer* out,buffer* in);

extern int buffer_putulong(buffer *b,unsigned long l);
extern int buffer_put8long(buffer *b,unsigned long l);
extern int buffer_putxlong(buffer *b,unsigned long l);
extern int buffer_putlong(buffer *b,unsigned long l);

extern buffer *buffer_0;
extern buffer *buffer_0small;
extern buffer *buffer_1;
extern buffer *buffer_1small;
extern buffer *buffer_2;

#ifdef STRALLOC_H
/* write stralloc to buffer */
extern int buffer_putsa(buffer* b,stralloc* sa);

/* these "read token" functions return 0 if the token was complete or
 * EOF was hit or -1 on error.  In contrast to the non-stralloc token
 * functions, the separator is also put in the stralloc; use
 * stralloc_chop or stralloc_chomp to get rid of it. */

/* WARNING!  These token reading functions will not clear the stralloc!
 * They _append_ the token to the contents of the stralloc.  The idea is
 * that this way these functions can be used on non-blocking sockets;
 * when you get signalled EAGAIN, just call the functions again when new
 * data is available. */

/* read token from buffer to stralloc */
extern int buffer_get_token_sa(buffer* b,stralloc* sa,const char* charset,unsigned int setlen);
/* read line from buffer to stralloc */
extern int buffer_getline_sa(buffer* b,stralloc* sa);

typedef int (*sa_predicate)(stralloc* sa);

/* like buffer_get_token_sa but the token ends when your predicate says so */
extern int buffer_get_token_sa_pred(buffer* b,stralloc* sa,sa_predicate p);
#endif

#endif
