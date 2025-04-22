/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef BUFFER_H
#define BUFFER_H

/* for size_t: */
#include <stddef.h>
/* for ssize_t: */
#include <sys/types.h>
/* for strlen */
#include <string.h>

#include <libowfat/compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/* NOTE: unlike stdio a buffer can always only be for reading OR for
 * writing, not both at the same time! Also, there is no linked flushing
 * logic (reading from buffer_0 won't implicitly flush buffer_1 first */
typedef struct buffer {
  char *x;		/* actual buffer space */
  size_t p;		/* current position */
  size_t n;		/* current size of string in buffer */
  size_t a;		/* allocated buffer size */
  union {
    ssize_t (*wop)(int fd,const void* buf,size_t len);	// for write(2)
    ssize_t (*rop)(int fd,void* buf,size_t len);	// for read(2)
    ssize_t (*wopc)(int fd,const char* buf,size_t len, void* cookie);	// write(2) w/ cookie
    ssize_t (*ropc)(int fd,char* buf,size_t len, void* cookie);	// read(2) w/ cookie
  } op;
  void* cookie;			/* used internally by the to-stralloc buffers, and for buffer chaining */
  void (*deinit)(void*);	/* called to munmap/free cleanup, with a pointer to the buffer as argument */
  int fd;		/* passed as first argument to op */
} buffer;

#define BUFFER_INIT(func,filedes,buf,len) { .x=(buf), .p=0, .n=0, .a=(len), .op.wop=(func), .cookie=NULL, .deinit=NULL, .fd=(filedes) }
#define BUFFER_INIT_FREE(func,filedes,buf,len) { .x=(buf), .p=0, .n=0, .a=(len), .op.wop=(func), .cookie=NULL, .deinit=buffer_free, .fd=(filedes) }
#define BUFFER_INIT_READ(func,filedes,buf,len) { .x=(buf), .p=0, .n=0, .a=(len), .op.rop=(func), .cookie=NULL, .deinit=NULL, .fd=(filedes) }
#define BUFFER_INSIZE 8192
#define BUFFER_OUTSIZE 8192

/* Initialize a buffer with an existing memory area, which the buffer
 * will NOT take ownership of (i.e. won't free the memory when it's done) */
__bufout(4,5)
void buffer_init(buffer* b, ssize_t (*op)(int fd,const void* buf,size_t l), int fd, char* y, size_t ylen);

__bufout(4,5)
void buffer_init_forread(buffer* b, ssize_t (*op)(int fd,void* buf,size_t l), int fd, char* y, size_t ylen);

/* Initialize a buffer with an existing memory area, which the buffer
 * WILL take ownership of (it will call free() on it when it's done) */
__bufout(4,5)
att_free(malloc,4)
void buffer_init_free(buffer* b,ssize_t (*op)(int fd,const void* buf,size_t l),int fd,char* y,size_t ylen);

__bufout(4,5)
att_free(malloc,4)
void buffer_init_free_forread(buffer* b,ssize_t (*op)(int fd,void* buf,size_t l),int fd,char* y,size_t ylen);

/* Call buffer_init with op=read(), return 0.
 * If fd==-1, return -1 instead, leaving b untouched. */
__bufout(3,4)
int buffer_init_read(buffer* b, int fd, char* y, size_t ylen);

/* Call buffer_init with op=write(), return 0.
 * If fd==-1, return -1 instead, leaving b untouched. */
__bufout(3,4)
int buffer_init_write(buffer* b, int fd, char* y, size_t ylen);

/* Will allocate a buffer of size ylen and then call
 * buffer_init_free(b, op, fd, thebuffer, ylen) on it.
 * Returns 0 on success, -1 on error (setting errno).
 * Passing fd==-1 is treated as error (so you can pass open*() for fd). */
int buffer_init_allocbuf(buffer* b, ssize_t (*op)(int fd,const void* buf,size_t l), int fd, size_t ylen);

int buffer_init_allocbuf_forread(buffer* b, ssize_t (*op)(int fd,void* buf,size_t l), int fd, size_t ylen);

/* Call buffer_init_allocbuf with op=read */
int buffer_init_read_allocbuf(buffer* b, int fd, size_t ylen);

/* Call buffer_init_allocbuf with op=read */
int buffer_init_write_allocbuf(buffer* b, int fd, size_t ylen);



/* Initialize a buffer without actual I/O.
 * You give it a pre-existing memory area.
 * When reading from this buffer, it will simply return the data from
 * that memory area. If it reaches the end, it will signal EOF and never
 * actually attempt to read from any actual file.
 * Does not take ownership. Useful for testing. */
__bufin(2,3)
void buffer_init_staticcontents(buffer* b,char* y,size_t ylen);

__bufin(2,3)
void buffer_init_staticcontents_forread(buffer* b,const char* y,size_t ylen);

/* Same but the buffer takes ownership of the static buffer and frees it
 * in buffer_close. */
__bufin(2,3)
att_free(malloc,2)
void buffer_init_staticcontents_free(buffer* b,char* y,size_t ylen);


/* Set buffer->deinit to this if you want buffer_close() to call free() on
 * the associated memory buffer */
att_free(malloc,1)
void buffer_free(void* buf);

/* Set buffer->deinit to this if you want buffer_close() to call munmap() on
 * the associated memory buffer */
void buffer_munmap(void* buf);

/* Initialize a buffer so it will read from this file by memory mapping
 * the whole thing. */
__strin(2)
int buffer_mmapread(buffer* b,const char* filename);

/* Indicate you are done with a buffer.
 * If the buffer has an associated memory map or buffer it owns, it will
 * free that memory. This will NOT call free() on b itself! */
void buffer_close(buffer* b);

/* Flush the buffer. This is only meaningful for write buffers and will
 * cause a write() syscall (or whatever you set as buffer->op) for all
 * the data in the buffer. */
int buffer_flush(buffer* b);

__bufin(2,3)
int buffer_put(buffer* b,const char* x,size_t len);

__bufin(2,3)
int buffer_putalign(buffer* b,const char* x,size_t len);

__bufin(2,3)
int buffer_putflush(buffer* b,const char* x,size_t len);

__strin(2)
int buffer_puts(buffer* b,const char* x);

__strin(2)
int buffer_putsalign(buffer* b,const char* x);

__strin(2)
int buffer_putsflush(buffer* b,const char* x);

#if defined(__GNUC__) && !defined(__LIBOWFAT_INTERNAL)
/* as a little gcc-specific hack, if somebody calls buffer_puts with a
 * constant string, where we know its length at compile-time, call
 * buffer_put with the known length instead */
#define buffer_puts(b,s) (__builtin_constant_p(s) ? buffer_put(b,s,strlen(s)) : buffer_puts(b,s))
#define buffer_putsflush(b,s) (__builtin_constant_p(s) ? buffer_putflush(b,s,strlen(s)) : buffer_putsflush(b,s))
#endif

int buffer_putm_internal(buffer* b,...);
int buffer_putm_internal_flush(buffer* b,...);
#define buffer_putm(b,...) buffer_putm_internal(b,__VA_ARGS__,(char*)0)
#define buffer_putmflush(b,...) buffer_putm_internal_flush(b,__VA_ARGS__,(char*)0)

int buffer_putspace(buffer* b);
int buffer_putnlflush(buffer* b); /* put \n and flush */

#define buffer_PUTC(s,c) \
  ( ((s)->a != (s)->p) \
    ? ( (s)->x[(s)->p++] = (c), 0 ) \
    : buffer_put((s),&(c),1) \
  )

__bufout(2,3)
ssize_t buffer_get(buffer* b,char* x,size_t len);
ssize_t buffer_feed(buffer* b);
att_write(2) att_nonnull(2)
ssize_t buffer_getc(buffer* b,char* x);
att_write(2) att_nonnull(2)
ssize_t buffer_peekc(buffer* b,char* x);
__bufout(2,3)
ssize_t buffer_getn(buffer* b,char* x,size_t len);

/* read bytes until the destination buffer is full (len bytes), end of
 * file is reached or the read char is in charset (setlen bytes).  An
 * empty line when looking for \n will write '\n' to x and return 0.  If
 * EOF is reached, \0 is written to the buffer */
__bufout(2,3)
__bufin(4,5)
ssize_t buffer_get_token(buffer* b,char* x,size_t len,const char* charset,size_t setlen);
__bufout(2,3)
ssize_t buffer_getline(buffer* b,char* x,size_t len);

/* this predicate is given the string as currently read from the buffer
 * and is supposed to return 1 if the token is complete, 0 if not. */
typedef int (*string_predicate)(const char* x,size_t len);

/* like buffer_get_token but the token ends when your predicate says so */
__bufout(2,3)
ssize_t buffer_get_token_pred(buffer* b,char* x,size_t len,string_predicate p);

char *buffer_peek(buffer* b);
ssize_t buffer_seek(buffer* b,size_t len);

#define buffer_PEEK(s) ( (s)->x + (s)->p )
#define buffer_SEEK(s,len) ( (s)->p += (len) )

#define buffer_GETC(s,c) \
  ( ((s)->p < (s)->n) \
    ? ( *(c) = *buffer_PEEK(s), buffer_SEEK((s),1), 1 ) \
    : buffer_get((s),(c),1) \
  )

int buffer_putulong(buffer *b,unsigned long int l);
int buffer_put8long(buffer *b,unsigned long int l);
int buffer_putxlong(buffer *b,unsigned long int l);
int buffer_putlong(buffer *b,signed long int l);

int buffer_putlonglong(buffer* b,signed long long int l);
int buffer_putulonglong(buffer* b,unsigned long long int l);

int buffer_puterror(buffer* b);
int buffer_puterror2(buffer* b, int errnum);

extern buffer *buffer_0;
extern buffer *buffer_0small;
extern buffer *buffer_1;
extern buffer *buffer_1small;
extern buffer *buffer_2;

#ifdef STRALLOC_H
/* write stralloc to buffer */
int buffer_putsa(buffer* b,const stralloc* sa);
/* write stralloc to buffer and flush */
int buffer_putsaflush(buffer* b,const stralloc* sa);

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
__bufin(3,4)
int buffer_get_token_sa(buffer* b,stralloc* sa,const char* charset,size_t setlen);
/* read line from buffer to stralloc */
int buffer_getline_sa(buffer* b,stralloc* sa);

/* same as buffer_get_token_sa but empty sa first */
__bufin(3,4)
int buffer_get_new_token_sa(buffer* b,stralloc* sa,const char* charset,size_t setlen);
/* same as buffer_getline_sa but empty sa first */
int buffer_getnewline_sa(buffer* b,stralloc* sa);

typedef int (*sa_predicate)(stralloc* sa);

/* like buffer_get_token_sa but the token ends when your predicate says so */
int buffer_get_token_sa_pred(buffer* b,stralloc* sa,sa_predicate p);
/* same, but clear sa first */
int buffer_get_new_token_sa_pred(buffer* b,stralloc* sa,sa_predicate p);

/* make a buffer from a stralloc.
 * Do not change the stralloc after this! */
void buffer_fromsa(buffer* b,const stralloc* sa);	/* read from sa */
int buffer_tosa(buffer*b,stralloc* sa);		/* write to sa, auto-growing it */
#endif

__bufin(2,3)
void buffer_frombuf(buffer* b,const char* x,size_t l);	/* buffer reads from static buffer */

#ifdef ARRAY_H
void buffer_fromarray(buffer* b,array* a);	/* buffer reads from array */
#endif

#ifdef __cplusplus
}
#endif

#endif
