/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef MMAP_H
#define MMAP_H

#include <stddef.h>

#include <libowfat/compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/* open file for reading, mmap whole file, close file, write length of
 * map in filesize and return pointer to map. */
__strin(1) att_write(2)
const char* mmap_read(const char *filename,size_t* filesize);

/* like mmap_read but use openat instead of open */
__strin(1) att_write(2)
const char* mmap_readat(const char *filename,size_t* filesize,int dirfd);

/* open file for reading, mmap whole file privately (copy on write),
 * close file, write length of map in filesize and return pointer to
 * map. */
__strin(1) att_write(2)
char* mmap_private(const char *filename,size_t* filesize);

/* open file for writing, mmap whole file shared, close file, write
 * length of map in filesize and return pointer to map. */
__strin(1) att_write(2)
char* mmap_shared(const char *filename,size_t* filesize);

/* unmap a mapped region */
__strin(1)
int mmap_unmap(const char* mapped,size_t maplen);

#ifdef __cplusplus
}
#endif

#endif
