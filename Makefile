all: t byte.a fmt.a scan.a str.a uint.a open.a stralloc.a unix.a socket.a

VPATH=str:byte:fmt:scan:uint:open:stralloc:unix:socket

CC=egcc
#CFLAGS=-I. -pipe -Wall -Os -march=pentiumpro -fomit-frame-pointer -fschedule-insns2 -Wall
CFLAGS=-I. -I../dietlibc/include -pipe -Wall -g #-Os -march=athlon -mcpu=athlon -fomit-frame-pointer -fschedule-insns2
#CFLAGS=-I../dietlibc/include -I. -pipe -Wall -Os -march=pentiumpro -mcpu=athlon -fomit-frame-pointer -fschedule-insns2 -Wall
#CFLAGS=-I../dietlibc/include -pipe -Os -march=pentiumpro -mcpu=pentiumpro -fomit-frame-pointer -fschedule-insns2 -Wall

BYTE_OBJS=$(patsubst byte/%.c,%.o,$(wildcard byte/*.c))
FMT_OBJS=$(patsubst fmt/%.c,%.o,$(wildcard fmt/*.c))
SCAN_OBJS=$(patsubst scan/%.c,%.o,$(wildcard scan/*.c))
STR_OBJS=$(patsubst str/%.c,%.o,$(wildcard str/*.c))
UINT_OBJS=$(patsubst uint/%.c,%.o,$(wildcard uint/*.c))
OPEN_OBJS=$(patsubst open/%.c,%.o,$(wildcard open/*.c))
STRA_OBJS=$(patsubst stralloc/%.c,%.o,$(wildcard stralloc/*.c))
UNIX_OBJS=$(patsubst unix/%.c,%.o,$(wildcard unix/*.c))
SOCKET_OBJS=$(patsubst socket/%.c,%.o,$(wildcard socket/*.c))

$(BYTE_OBJS): byte.h
$(FMT_OBJS): fmt.h
$(SCAN_OBJS): scan.h
$(STR_OBJS): str.h
$(UINT_OBJS): uint16.h uint32.h
$(STRA_OBJS): stralloc.h
$(SOCKET_OBJS): socket.h

byte.a: $(BYTE_OBJS)
fmt.a: $(FMT_OBJS)
scan.a: $(SCAN_OBJS)
str.a: $(STR_OBJS)
uint.a: $(UINT_OBJS)
open.a: $(OPEN_OBJS)
stralloc.a: $(STRA_OBJS)
unix.a: $(UNIX_OBJS)
socket.a: $(SOCKET_OBJS)

%.a:
	ar cr $@ $^

t: t.o socket.a stralloc.a str.a fmt.a scan.a str.a uint.a open.a byte.a
	gcc -g -o $@ $^

.PHONY: clean tar
clean:
	rm -f *.o *.a core t

tar:
	cd .. && tar cf libowfat.tar.bz2 libowfat --use=bzip2 --exclude CVS
