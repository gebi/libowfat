all: t byte.a fmt.a scan.a str.a uint.a open.a stralloc.a unix.a socket.a buffer.a mmap.a libowfat.a

VPATH=str:byte:fmt:scan:uint:open:stralloc:unix:socket:buffer:mmap

CC=egcc
#CFLAGS=-I. -pipe -Wall -Os -march=pentiumpro -fomit-frame-pointer -fschedule-insns2 -Wall
CFLAGS=-I. -I../dietlibc/include -pipe -Wall -Os -march=athlon -mcpu=athlon -malign-functions=2 -fomit-frame-pointer -fschedule-insns2 -g
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
BUFFER_OBJS=$(patsubst buffer/%.c,%.o,$(wildcard buffer/*.c))
MMAP_OBJS=$(patsubst mmap/%.c,%.o,$(wildcard mmap/*.c))

$(BYTE_OBJS): byte.h
$(FMT_OBJS): fmt.h
$(SCAN_OBJS): scan.h
$(STR_OBJS): str.h
$(UINT_OBJS): uint16.h uint32.h
$(STRA_OBJS): stralloc.h
$(SOCKET_OBJS): socket.h
$(BUFFER_OBJS): buffer.h
$(MMAP_OBJS): mmap.h

byte.a: $(BYTE_OBJS)
fmt.a: $(FMT_OBJS)
scan.a: $(SCAN_OBJS)
str.a: $(STR_OBJS)
uint.a: $(UINT_OBJS)
open.a: $(OPEN_OBJS)
stralloc.a: $(STRA_OBJS)
unix.a: $(UNIX_OBJS)
socket.a: $(SOCKET_OBJS)
buffer.a: $(BUFFER_OBJS)
mmap.a: $(MMAP_OBJS)

libowfat.a: $(BYTE_OBJS) $(FMT_OBJS) $(SCAN_OBJS) $(STR_OBJS) \
$(UINT_OBJS) $(OPEN_OBJS) $(STRA_OBJS) $(UNIX_OBJS) $(SOCKET_OBJS) \
$(BUFFER_OBJS) $(MMAP_OBJS)

%.a:
	ar cr $@ $^

t: t.o socket.a stralloc.a fmt.a scan.a uint.a mmap.a open.a buffer.a \
str.a byte.a
	gcc -g -o $@ $^

.PHONY: clean tar
clean:
	rm -f *.o *.a core t

tar:
	cd .. && tar cf libowfat.tar.bz2 libowfat --use=bzip2 --exclude CVS
