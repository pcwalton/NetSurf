AR = ar
CC = gcc
LD = gcc
DOXYGEN = doxygen
INSTALL = install
SED = sed
MKDIR = mkdir
PKG_CONFIG = pkg-config

ARFLAGS = -cru
CFLAGS = -g -Wall -Wextra -Wundef -Wpointer-arith -Wcast-align \
	-Wwrite-strings -Wstrict-prototypes \
	-Wnested-externs -Werror -pedantic -std=c99 \
	-Wno-format-zero-length -Wformat-security -Wstrict-aliasing=2 \
	-Wmissing-format-attribute -Wunused -Wunreachable-code \
	-Wformat=2 -Werror-implicit-function-declaration \
	-Wmissing-declarations -Wmissing-prototypes
LDFLAGS = -g -L./

# Installation prefix, if not already defined (e.g. on command line)
PREFIX ?= /usr/local
DESTDIR ?=

.PHONY: all clean docs install uninstall

all: libnsbmp.a bin/decode_bmp bin/decode_ico
	
libnsbmp.a: libnsbmp.o libnsbmp.pc
	${AR} ${ARFLAGS} libnsbmp.a libnsbmp.o

libnsbmp.pc: libnsbmp.pc.in
	$(SED) -e 's#PREFIX#$(PREFIX)#' libnsbmp.pc.in > libnsbmp.pc

%.o: %.c
	${CC} -c ${CFLAGS} -o $@ $<

bin/decode_bmp: examples/decode_bmp.c libnsbmp.a
	${CC} ${CFLAGS} -o $@ $< libnsbmp.a

bin/decode_ico: examples/decode_ico.c libnsbmp.a
	${CC} ${CFLAGS} -o $@ $< libnsbmp.a

docs:
	${DOXYGEN}

clean:
	rm -f $(wildcard *.o) $(wildcard *.a) libnsbmp.pc
	rm -rf doc

install: libnsbmp.a libnsbmp.pc
	$(MKDIR) -p $(DESTDIR)$(PREFIX)/lib/pkgconfig
	$(MKDIR) -p $(DESTDIR)$(PREFIX)/lib
	$(MKDIR) -p $(DESTDIR)$(PREFIX)/include
	$(INSTALL) --mode=644 -t $(DESTDIR)$(PREFIX)/lib libnsbmp.a
	$(INSTALL) --mode=644 -t $(DESTDIR)$(PREFIX)/include libnsbmp.h
	$(INSTALL) --mode=644 -t $(DESTDIR)$(PREFIX)/lib/pkgconfig libnsbmp.pc

uninstall:
	rm $(DESTDIR)$(PREFIX)/lib/libnsbmp.a
	rm $(DESTDIR)$(PREFIX)/include/libnsbmp.h
	rm $(DESTDIR)$(PREFIX)/lib/pkgconfig/libnsbmp.pc
