/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef UINT32_H
#define UINT32_H

#include <inttypes.h>

typedef uint32_t uint32;
typedef int32_t int32;

#if (defined(__i386__) || defined(__x86_64__)) && !defined(NO_UINT32_MACROS)
#define uint32_pack(out,in) (*(uint32*)(out)=(in))
#define uint32_unpack(in,out) (*(out)=*(uint32*)(in))
#define uint32_read(in) (*(uint32*)(in))
void uint32_pack_big(char *out,uint32 in);
void uint32_unpack_big(const char *in,uint32* out);
uint32 uint32_read_big(const char *in);
#else

void uint32_pack(char *out,uint32 in);
void uint32_pack_big(char *out,uint32 in);
void uint32_unpack(const char *in,uint32* out);
void uint32_unpack_big(const char *in,uint32* out);
uint32 uint32_read(const char *in);
uint32 uint32_read_big(const char *in);

#endif

#endif
