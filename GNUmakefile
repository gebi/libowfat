# Warning!  There is no real "make install" support.
# The include files are named after the djb, i.e. pretty generic.  They
# will probably conflict with other headers.  That's why I install them
# in /opt/diet, where they are in the default search path for my diet libc
# work but don't conflict with anything there.  YMMV.

prefix=/opt/diet
LIBDIR=${prefix}/lib
INCLUDEDIR=${prefix}/include
MAN3DIR=${prefix}/man/man3

LIBS=byte.a fmt.a scan.a str.a uint.a open.a stralloc.a unix.a socket.a \
buffer.a mmap.a taia.a tai.a dns.a case.a mult.a array.a io.a

all: t $(LIBS) libowfat.a

# comment out the following line if you don't want to build with the
# diet libc (http://www.fefe.de/dietlibc/).
DIET=/opt/diet/bin/diet -Os
CC=gcc
CFLAGS=-pipe -Wall -O2 -fomit-frame-pointer
#CFLAGS=-pipe -Os -march=pentiumpro -mcpu=pentiumpro -fomit-frame-pointer -fschedule-insns2 -Wall

# startrip
VPATH=str:byte:fmt:scan:uint:open:stralloc:unix:socket:buffer:mmap:textcode:taia:tai:dns:case:array:mult:io

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
TEXTCODE_OBJS=$(patsubst textcode/%.c,%.o,$(wildcard textcode/*.c))
TAI_OBJS=$(patsubst tai/%.c,%.o,$(wildcard tai/*.c))
TAIA_OBJS=$(patsubst taia/%.c,%.o,$(wildcard taia/*.c))
DNS_OBJS=$(patsubst dns/%.c,%.o,$(wildcard dns/*.c))
CASE_OBJS=$(patsubst case/%.c,%.o,$(wildcard case/*.c))
ARRAY_OBJS=$(patsubst array/%.c,%.o,$(wildcard array/*.c))
MULT_OBJS=$(patsubst mult/%.c,%.o,$(wildcard mult/*.c))
IO_OBJS=$(patsubst io/%.c,%.o,$(wildcard io/*.c))

$(BYTE_OBJS): byte.h
$(FMT_OBJS): fmt.h
$(SCAN_OBJS): scan.h
$(STR_OBJS): str.h
$(UINT_OBJS): uint16.h uint32.h
$(STRA_OBJS): stralloc.h
$(SOCKET_OBJS): socket.h
$(BUFFER_OBJS): buffer.h
$(MMAP_OBJS): mmap.h open.h
$(TEXTCODE_OBJS): textcode.h
$(TAI_OBJS): tai.h uint64.h
$(TAIA_OBJS): taia.h tai.h uint64.h
$(DNS_OBJS): dns.h stralloc.h taia.h tai.h uint64.h iopause.h
$(CASE_OBJS): case.h
$(ARRAY_OBJS): uint64.h array.h
$(MULT_OBJS): uint64.h uint32.h uint16.h safemult.h
$(IO_OBJS): uint64.h array.h io.h io_internal.h taia.h tai.h

iopause.o: select.h
openreadclose.o readclose.o: readclose.h
dns_rcip.o dns_rcrw.o openreadclose.o: openreadclose.h
# stoprip

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
textcode.a: $(TEXTCODE_OBJS)
taia.a: $(TAIA_OBJS)
tai.a: $(TAI_OBJS)
dns.a: $(DNS_OBJS)
case.a: $(CASE_OBJS)
array.a: $(ARRAY_OBJS)
mult.a: $(MULT_OBJS)
io.a: $(IO_OBJS)

libowfat.a: $(DNS_OBJS) $(BYTE_OBJS) $(FMT_OBJS) $(SCAN_OBJS) \
$(STR_OBJS) $(UINT_OBJS) $(OPEN_OBJS) $(STRA_OBJS) $(UNIX_OBJS) \
$(SOCKET_OBJS) $(BUFFER_OBJS) $(MMAP_OBJS) $(TEXTCODE_OBJS) \
$(TAIA_OBJS) $(TAI_OBJS) $(CASE_OBJS) $(ARRAY_OBJS) $(MULT_OBJS) \
$(IO_OBJS)

%.o: %.c
	$(DIET) $(CC) -c $< -o $@ -I. $(CFLAGS)

%.a:
	ar cr $@ $^
	-ranlib $@

t.o: iopause.h

t: t.o libowfat.a
	$(DIET) $(CC) -g -o $@ $^

.PHONY: all clean tar install rename
clean:
	rm -f *.o *.a *.da *.bbg *.bb core t haveip6.h haven2i.h havesl.h haveinline.h \
iopause.h select.h havekqueue.h haveepoll.h libepoll Makefile

INCLUDES=buffer.h byte.h fmt.h ip4.h ip6.h mmap.h scan.h socket.h str.h stralloc.h \
uint16.h uint32.h uint64.h open.h textcode.h tai.h taia.h dns.h iopause.h case.h \
openreadclose.h readclose.h ndelay.h array.h io.h safemult.h

install: libowfat.a
	install -d $(INCLUDEDIR) $(MAN3DIR) $(LIBDIR)
	install -m 644 $(INCLUDES) $(INCLUDEDIR)
	install -m 644 $(wildcard */*.3) $(MAN3DIR)
	install -m 644 libowfat.a $(LIBDIR)

uninstall:
	rm -f $(patsubst %.h,$(INCLUDEDIR)/%.h,$(INCLUDES))
	rm -f $(patsubst %.3,$(MAN3DIR)/%.3,$(notdir $(wildcard */*.3)))
	rm -f $(LIBDIR)/libowfat.a

VERSION=libowfat-$(shell head -1 CHANGES|sed 's/://')
CURNAME=$(notdir $(shell pwd))

tar: Makefile clean rename
	rm -f dep libdep
	cd ..; tar cvvf $(VERSION).tar.bz2 $(VERSION) --use=bzip2 --exclude CVS

rename:
	if test $(CURNAME) != $(VERSION); then cd .. && mv $(CURNAME) $(VERSION); fi

haveip6.h: tryip6.c
	-rm -f $@
	if $(DIET) $(CC) $(CFLAGS) -c tryip6.c >/dev/null 2>&1; then echo "#define LIBC_HAS_IP6"; fi > $@
	-rm -f tryip6.o

haven2i.h: tryn2i.c
	-rm -f $@
	if $(DIET) $(CC) $(CFLAGS) -o t tryn2i.c >/dev/null 2>&1; then echo "#define HAVE_N2I"; fi > $@
	-rm -f t

havesl.h: trysl.c
	-rm -f $@
	if ! $(DIET) $(CC) $(CFLAGS) -o t trysl.c >/dev/null 2>&1; then echo "typedef int socklen_t;"; fi > $@
	-rm -f t

haveinline.h: tryinline.c
	-rm -f $@
	if ! $(DIET) $(CC) $(CFLAGS) -c tryinline.c >/dev/null 2>&1; then echo "#define inline"; fi > $@
	-rm -f tryinline.o

havekqueue.h: trykqueue.c
	-rm -f $@
	if $(DIET) $(CC) $(CFLAGS) -c trykqueue.c >/dev/null 2>&1; then echo "#define HAVE_KQUEUE"; fi > $@
	-rm -f trykqueue.o

haveepoll.h: tryepoll.c
	-rm -f $@
	if $(DIET) $(CC) $(CFLAGS) -o tryepoll tryepoll.c >/dev/null 2>&1; then echo "#define HAVE_EPOLL 1"; else \
	if $(DIET) $(CC) $(CFLAGS) -o tryepoll tryepoll.c -lepoll >/dev/null 2>&1; then echo "#define HAVE_EPOLL 2"; fi; fi > $@
	-rm -f tryepoll

libepoll: haveepoll.h
	if test "z`cat haveepoll.h`" = "z#define HAVE_EPOLL 2"; then echo -lepoll; fi > $@

iopause.h: iopause.h1 iopause.h2 trypoll.c
	-rm -f $@
	if $(DIET) $(CC) $(CFLAGS) -o t trypoll.c >/dev/null 2>&1; then cp iopause.h2 iopause.h; else cp iopause.h1 iopause.h; fi
	-rm -f t

select.h: select.h1 select.h2 trysysel.c
	-rm -f $@
	if $(DIET) $(CC) $(CFLAGS) -c trysysel.c >/dev/null 2>&1; then cp select.h2 select.h; else cp select.h1 select.h; fi
	-rm -f trysysel.o


socket_accept6.o socket_connect6.o socket_local6.o socket_mchopcount6.o \
socket_mcjoin6.o socket_mcleave6.o socket_mcloop6.o socket_recv6.o \
socket_remote6.o socket_send6.o socket_tcp6.o socket_udp6.o: haveip6.h

socket_getifidx.o socket_getifname.o: haven2i.h

socket_accept4.o socket_accept6.o socket_connected.o socket_local4.o \
socket_local6.o socket_recv4.o socket_recv6.o socket_remote4.o \
socket_remote6.o: havesl.h

fmt_xlong.o scan_xlong.o fmt_ip6_flat.o $(TEXTCODE_OBJS): haveinline.h

dep: haveip6.h haven2i.h havesl.h haveinline.h iopause.h select.h
	gcc -I. -MM $(wildcard */*.c) t.c > dep

libdep:
	for i in $(LIBS); do (echo -n $$i|tr a-z A-Z|sed 's/.A$$/_OBJS=/'; echo $${i%.a}/*.c|sed -e 's@[^/]*/\([^.]*\)\.c@\1.o @g'); done > libdep

Makefile: GNUmakefile dep libdep
	echo "# do not edit!  edit GNUmakefile instead" > $@
	sed '/startrip/,$$d' < GNUmakefile >> $@
	cat dep libdep >> $@
	sed -e '1,/stoprip/d' -e 's/ %.c$$//' < GNUmakefile >> $@

