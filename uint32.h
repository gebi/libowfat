#ifndef UINT32_H
#define UINT32_H

typedef unsigned int uint32;
typedef   signed int  int32;

#if defined(__i386__) && !defined(NO_UINT32_MACROS)
#define uint32_pack(out,in) (*(uint32*)(out)=(in))
#define uint32_unpack(in,out) (*(out)=*(uint32*)(in))
#define uint32_read(in) (*(uint32*)(in))
extern void uint32_pack_big(char *out,uint32 in);
extern void uint32_unpack_big(const char *in,uint32* out);
extern uint32 uint32_read_big(const char *in);
#else

extern void uint32_pack(char *out,uint32 in);
extern void uint32_pack_big(char *out,uint32 in);
extern void uint32_unpack(const char *in,uint32* out);
extern void uint32_unpack_big(const char *in,uint32* out);
extern uint32 uint32_read(const char *in);
extern uint32 uint32_read_big(const char *in);

#endif

#endif
