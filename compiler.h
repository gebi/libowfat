#ifndef LIBOWFAT_ATTRIBUTES_H
#define LIBOWFAT_ATTRIBUTES_H

// This is here so you can use inline with older gcc versions
#if __GNUC__ && (__STDC_VERSION__ < 199901L)
#define inline __inline__
#endif

#ifndef __GNUC__
/* macro attribute declarations away if we don't have gcc or clang */
#define __attribute__(x)
#define __extension__
#endif

// This is here so we can put noexcept on function declarations in headers
#ifndef __cplusplus
#define noexcept
#endif

#define GCC_VERSION_ATLEAST(a,b) (__GNUC__ > a) || ((__GNUC__ == a) && (__GNUC_MINOR__ >= b))

#if GCC_VERSION_ATLEAST(2,95) && !defined(__STRICT_ANSI__)
#undef restrict
#define restrict __restrict
#else
#define restrict
#endif

// Use this to tell the compiler something is likely to be true or false
// The compiler can use that to give hints to the CPU and group error
// handling code to "cold" pages that never get paged in by the OS
#if GCC_VERSION_ATLEAST(3,0)
#define likely(a) __builtin_expect((a), 1)
#define unlikely(a) __builtin_expect((a), 0)
#else
#define likely(a) (a)
#define unlikely(a) (a)
#endif

// Function attribute for functions like abs() that depend only on the
// arguments and does not access any other memory in the program. The
// compiler can then optimize multiple calls on the same value into just
// one call.
#if GCC_VERSION_ATLEAST(2, 5)
#define att_const __attribute__((__const__))
#else
#define att_const
#endif

// Function attribute. Tells the compiler the function has no effect on
// the state of the program except for the return value, but unlike
// att_const the function can read other global state in the program,
// like the memory pointer arguments point to.
#if GCC_VERSION_ATLEAST(2, 96)
#define att_pure __attribute__((__pure__))
#else
#define att_pure
#endif

// Function attribute for functions that return freshly allocated
// memory. The compiler then knows the pointer does not alias with other
// pointers, which enables several optimizations.
#if GCC_VERSION_ATLEAST(3, 0)
#define att_malloc __attribute__((__malloc__))
#else
#define att_malloc
#endif

// Function attribute for functions that return an important value, like
// an error code, where it would be a security issue not to check the
// return value, for example link or rename. The compiler will then warn
// if the caller ignores the return value.
#if GCC_VERSION_ATLEAST(3, 4)
#define att_warn_unused_result __attribute__((__warn_unused_result__))
#else
#define att_warn_unused_result
#endif

// Function attributes. The compiler will put hot and cold functions
// adjacent in memory. If you mark error handling functions as
// cold, the compiler can arrange the binary so the operating system
// never reads those pages into main memory from disk.
#if GCC_VERSION_ATLEAST(4, 3)
#define att_hot __attribute__((__hot__))
#define att_cold __attribute__((__cold__))
#else
#define att_hot
#define att_cold
#endif

// Function attributes. att_alloc tells the compiler that the compiler
// returns a buffer of the size given in the function argument x
// (as an index, counted left to right, starting at 1). att_calloc says
// the buffer size is a*b (both again as an function argument index,
// counted from left to right, starting at 1)
// If we tell the compiler the buffer size, it can detect if we access
// values outside the buffer area.
#if GCC_VERSION_ATLEAST(4, 3)
#define att_alloc(x) __attribute__((alloc_size(x)))
#define att_calloc(x,y) __attribute__((alloc_size(x,y)))
#else
#define att_alloc
#define att_calloc
#endif

// Function attributes.
// att_read(1) says the function reads from the first function argument.
// att_readn(1,2) is like att_read but the buffer size is given as the second argument.
// att_write(1) says the function writes to the first function argument.
// att_writen(1) is like att_write but the buffer size is given as the second argument.
// att_mangle(1) says the function read/writes to the first function argument.
// att_manglen(1) is like att_mangle but the buffer size is given as the second argument.
#if GCC_VERSION_ATLEAST(10, 0)
#define att_read(argno_ptr) __attribute__((access(read_only, argno_ptr)))
#define att_readn(argno_ptr, argno_size) __attribute__((access(read_only, argno_ptr, argno_size)))
#define att_write(argno_ptr) __attribute__((access(write_only, argno_ptr)))
#define att_writen(argno_ptr, argno_size) __attribute__((access(write_only, argno_ptr, argno_size)))
#define att_mangle(argno_ptr) __attribute__((access(read_write, argno_ptr)))
#define att_manglen(argno_ptr, argno_size) __attribute__((access(read_write, argno_ptr, argno_size)))
#else
#define att_read(argno_ptr)
#define att_readn(argno_ptr, argno_size)
#define att_write(argno_ptr)
#define att_writen(argno_ptr, argno_size)
#define att_mangle(argno_ptr)
#define att_manglen(argno_ptr, argno_size)
#endif

// Function attribute. The compiler will warn if you call this function.
#if GCC_VERSION_ATLEAST(3, 2)
#define att_dontuse __attribute__((__deprecated__))
#else
#define att_dontuse
#endif

// Function attribute. Tells the compiler that the param's function
// argument can not be NULL (given as index of function arguments,
// counted left to right from 1)
#if GCC_VERSION_ATLEAST(3, 3)
#define att_nonnull(params) __attribute__((__nonnull__(params)))
#else
#define att_nonnull(params)
#endif

// Function attribute. Makes the compiler emit a warning or an error,
// abort compilation on error.
#if GCC_VERSION_ATLEAST(4, 3)
#define att_warn(message) __attribute__((__warning__(message)))
#define att_error(message) __attribute__((__warning__(message)))
#else
#define att_warn(message)
#define att_error(message)
#endif

// __builtin_constant_p is a GCC extension used in macros to handle
// cases differently where a macro argument is a constant
#ifndef __GNUC__
#define __builtin_constant_p(x) 0
#endif

#if defined(__GNUC__)
#if __has_attribute(null_terminated_string_arg)
// Function arguments.
// __strin: Argument idx (counted left to right from 1) is a non-NULL string the function reads
#define __strin(idx) __attribute__((nonnull(idx),access(read_only,idx),null_terminated_string_arg(idx)))
// __strnin: Argument idx (counted left to right from 1) is a non-NULL string the function reads, length limit in array elements given in the szidx'th argument
#define __strnin(idx,szidx) __attribute__((nonnull(idx),access(read_only,idx,szidx),null_terminated_string_arg(idx)))
// __bufin: Argument idx (counted left to right from 1) is a non-NULL buffer the function reads from, length limit in array elements given in the szidx'th argument
#define __bufin(bufidx,szidx) __attribute__((nonnull(bufidx),access(read_only,bufidx,szidx)))
// __strout: Argument idx (counted left to right from 1) is a non-NULL string the function writes
// Currently we can't tell the compiler that the output will be 0-terminated.
#define __strout(idx) __attribute__((nonnull(idx),access(write_only,idx)))
// __strnout: Argument idx (counted left to right from 1) is a non-NULL string the function writes, buffer size given in szidx'th function argument (in array elements, not necessarily bytes)
// Currently we can't tell the compiler that the output will be 0-terminated.
#define __strnout(idx,szidx) __attribute__((nonnull(idx),access(write_only,idx,szidx)))
// __bufout: Argument idx (counted left to right from 1) is a non-NULL buffer the function writes, buffer size given in szidx'th function argument (in array elements, not necessarily bytes)
#define __bufout(bufidx,szidx) __attribute__((nonnull(bufidx),access(write_only,bufidx,szidx)))
// __strinout: Argument idx (counted left to right from 1) is a non-NULL string the function reads and writes
// Currently we can't tell the compiler that the output will be 0-terminated.
#define __strinout(idx) __attribute__((nonnull(idx),access(read_write,idx),null_terminated_string_arg(idx)))
// __strninout: Argument idx (counted left to right from 1) is a non-NULL string the function reads and writes, buffer size in szidx'th function argument (in array elements, not necessarily bytes)
// Currently we can't tell the compiler that the output will be 0-terminated.
#define __strninout(idx,szidx) __attribute__((nonnull(idx),access(read_write,idx,szidx),null_terminated_string_arg(idx)))
// __bufinout: Argument idx (counted left to right from 1) is a non-NULL buffer the function reads and writes, buffer size in szidx'th function argument (in array elements, not necessarily bytes)
#define __bufinout(bufidx,szidx) __attribute__((nonnull(bufidx),access(read_write,bufidx,szidx)))
#elif GCC_VERSION_ATLEAST(10, 0)
#define __strin(idx) __attribute__((nonnull(idx),access(read_only,idx)))
#define __strnin(idx,szidx) __attribute__((nonnull(idx),access(read_only,idx,szidx)))
#define __bufin(bufidx,szidx) __attribute__((nonnull(bufidx),access(read_only,bufidx,szidx)))
#define __strout(idx) __attribute__((nonnull(idx),access(write_only,idx)))
#define __strnout(idx,szidx) __attribute__((nonnull(idx),access(write_only,idx,szidx)))
#define __bufout(bufidx,szidx) __attribute__((nonnull(bufidx),access(write_only,bufidx,szidx)))
#define __strinout(idx) __attribute__((nonnull(idx),access(read_write,idx)))
#define __strninout(idx,szidx) __attribute__((nonnull(idx),access(read_write,idx,szidx)))
#define __bufinout(bufidx,szidx) __attribute__((nonnull(bufidx),access(read_write,bufidx,szidx)))

#elif GCC_VERSION_ATLEAST(3, 3)

#define __strin(idx) __attribute__((nonnull(idx)))
#define __strnin(idx,szidx) __attribute__((nonnull(idx)))
#define __bufin(bufidx,szidx) __attribute__((nonnull(bufidx)))
#define __strout(idx) __attribute__((nonnull(idx)))
#define __strnout(idx,szidx) __attribute__((nonnull(idx)))
#define __bufout(bufidx,szidx) __attribute__((nonnull(bufidx)))
#define __strinout(idx) __attribute__((nonnull(idx)))
#define __strninout(idx,szidx) __attribute__((nonnull(idx)))
#define __bufinout(bufidx,szidx) __attribute__((nonnull(bufidx)))

#endif
#endif	// !defined(__GNUC__)

#endif
