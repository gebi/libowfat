#ifdef __MINGW32__

/* set errno to WSAGetLastError() */
int winsock2errno(long l);
void __winsock_init(void);

#else

#define winsock2errno(fnord) (fnord)
#define __winsock_init()

#endif
