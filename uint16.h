#ifndef UINT16_H
#define UINT16_H

#include <sys/cdefs.h>

typedef unsigned short uint16;

#ifdef NO_UINT16_MACROS
extern void uint16_pack(char *out,uint16 in) __THROW;
extern void uint16_pack_big(char *out,uint16 in) __THROW;
extern void uint16_unpack(const char *in,uint16* out) __THROW;
extern void uint16_unpack_big(const char *in,uint16* out) __THROW;
#else

#include <endian.h>

#if __BYTE_ORDER == __LITTLE_ENDIAN

#define uint16_pack(out,in) (*(short*)(out)=(in))

extern void uint16_pack_big(char *out,uint16 in) __THROW;

#define uint16_unpack(in,out) (*(out)=*(short*)(in))

extern void uint16_unpack_big(const char *in,uint16* out) __THROW;

#else

extern void uint16_pack(char *out,uint16 in) __THROW;

#define uint16_pack_big(out,in) (*(short*)(out)=(in))

extern void uint16_unpack(const char *in,uint16* out) __THROW;

#define uint16_unpack_big(in,out) (*(out)=*(short*)(in))

#endif

#endif

#endif
