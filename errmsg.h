#ifndef ERRMSG_H
#define ERRMSG_H

#ifdef __dietlibc__
#include <sys/cdefs.h>
#else
#define __attribute__(x)
#endif
/* for exit(): */
#include <stdlib.h>

/* These use file descriptor 2, not buffer_2!
 * Call buffer_flush(buffer_2) before calling these! */

extern const char* argv0;

void errmsg_iam(const char* who);	/* set argv0 */

/* terminate with NULL. */
/* newline is appended automatically. */
void errmsg_warn(const char* message, ...);
void errmsg_warnsys(const char* message, ...);

#define carp(...) errmsg_warn(__VA_ARGS__,0)
#define carpsys(...) errmsg_warnsys(__VA_ARGS__,0)
#define die(n,...) { errmsg_warn(__VA_ARGS__,0); exit(n); }
#define diesys(n,...) { errmsg_warnsys(__VA_ARGS__,0); exit(n); }

#endif
