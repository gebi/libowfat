#ifndef UINT32_H
#define UINT32_H

#include <sys/cdefs.h>

typedef unsigned int uint32;

#ifdef NO_UINT32_MACROS
extern void uint32_pack(char *out,uint32 in) __THROW;
extern void uint32_pack_big(char *out,uint32 in) __THROW;
extern void uint32_unpack(const char *in,uint32* out) __THROW;
extern void uint32_unpack_big(const char *in,uint32* out) __THROW;
#else

#include <endian.h>

#if __BYTE_ORDER == __LITTLE_ENDIAN

#define uint32_pack(out,in) (*(uint32*)(out)=(in))

extern void uint32_pack_big(char *out,uint32 in) __THROW;

#define uint32_unpack(in,out) (*(out)=*(uint32*)(in))

extern void uint32_unpack_big(const char *in,uint32* out) __THROW;

#else

extern void uint32_pack(char *out,uint32 in) __THROW;

#define uint32_pack_big(out,in) (*(uint32*)(out)=(in))

extern void uint32_unpack(const char *in,uint32* out) __THROW;

#define uint32_unpack_big(in,out) (*(out)=*(uint32*)(in))

#endif

#endif

#endif
