prefix=/usr/local
LIBDIR=${prefix}/lib
INCLUDEDIR=${prefix}/include
MAN3DIR=${prefix}/man/man3

all: t byte.a fmt.a scan.a str.a uint.a open.a stralloc.a unix.a socket.a buffer.a mmap.a libowfat.a

VPATH=str:byte:fmt:scan:uint:open:stralloc:unix:socket:buffer:mmap

# comment out the following line if you don't want to build with the
# diet libc (http://www.fefe.de/dietlibc/).
DIET=diet -Os
CC=gcc
CFLAGS=-I. -pipe -Wall -O2 -pipe -fomit-frame-pointer
#CFLAGS=-pipe -Os -march=pentiumpro -mcpu=pentiumpro -fomit-frame-pointer -fschedule-insns2 -Wall

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

%.o: %.c
	$(DIET) $(CC) -c $< -o $@ $(CFLAGS)

%.a:
	ar cr $@ $^
	-ranlib $@

t: t.o socket.a stralloc.a buffer.a scan.a uint.a mmap.a open.a fmt.a \
str.a byte.a
	$(DIET) $(CC) -g -o $@ $^

.PHONY: clean tar install rename
clean:
	rm -f *.o *.a *.da *.bbg *.bb core t haveip6.h haven2i.h havesl.h haveinline.h

install: libowfat.a
	install -d $(INCLUDEDIR) $(MAN3DIR) $(LIBDIR)
	install -m 644 buffer.h byte.h fmt.h ip4.h ip6.h mmap.h scan.h socket.h str.h stralloc.h uint16.h uint32.h uint64.h open.h $(INCLUDEDIR)
	install -m 644 $(wildcard */*.3) $(MAN3DIR)
	install -m 644 libowfat.a $(LIBDIR)

VERSION=libowfat-$(shell head -1 CHANGES|sed 's/://')
CURNAME=$(notdir $(shell pwd))

tar: clean rename
	cd ..; tar cvvf $(VERSION).tar.bz2 $(VERSION) --use=bzip2 --exclude CVS

rename:
	if test $(CURNAME) != $(VERSION); then cd .. && mv $(CURNAME) $(VERSION); fi

haveip6.h:
	-rm -f $@
	if $(DIET) $(CC) -c tryip6.c >/dev/null 2>&1; then echo "#define LIBC_HAS_IP6"; fi > $@
	-rm -f tryip6.o

haven2i.h:
	-rm -f $@
	if $(DIET) $(CC) -o t tryn2i.c >/dev/null 2>&1; then echo "#define HAVE_N2I"; fi > $@
	-rm -f t

havesl.h:
	-rm -f $@
	if ! $(DIET) $(CC) -o t trysl.c >/dev/null 2>&1; then echo "typedef int socklen_t;"; fi > $@
	-rm -f t

haveinline.h:
	-rm -f $@
	if ! $(DIET) $(CC) -c tryinline.c >/dev/null 2>&1; then echo "#define inline"; fi > $@
	-rm -f tryip6.o

socket_accept6.o socket_connect6.o socket_local6.o socket_mchopcount6.o \
socket_mcjoin6.o socket_mcleave6.o socket_mcloop6.o socket_recv6.o \
socket_remote6.o socket_send6.o socket_tcp6.o socket_udp6.o: haveip6.h

socket_getifidx.o socket_getifname.o: haven2i.h

socket_accept4.o socket_accept6.o socket_connected.o socket_local4.o \
socket_local6.o socket_recv4.o socket_recv6.o socket_remote4.o \
socket_remote6.o: havesl.h

fmt_xlong.o scan_xlong.o fmt_ip6_flat.o: haveinline.h
