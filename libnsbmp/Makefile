#
# This file is part of Libnsbmp
#

SOURCE = libnsbmp.c
HDRS = libnsbmp.h utils/log.h

CFLAGS = -Wall -Wextra -Wundef -Wpointer-arith -Wcast-align \
	-Wwrite-strings -Wstrict-prototypes \
	-Wnested-externs -pedantic -std=c99 \
	-Wno-format-zero-length -Wformat-security -Wstrict-aliasing=2 \
	-Wmissing-format-attribute -Wunused -Wunreachable-code \
	-Wformat=2 -Werror-implicit-function-declaration \
	-Wmissing-declarations -Wmissing-prototypes
ARFLAGS = -cr
INSTALL = install
SED = sed
DOXYGEN = doxygen

ifeq ($(TARGET),riscos)
GCCSDK_INSTALL_CROSSBIN ?= /home/riscos/cross/bin
GCCSDK_INSTALL_ENV ?= /home/riscos/env
CC = $(GCCSDK_INSTALL_CROSSBIN)/gcc
AR = $(GCCSDK_INSTALL_CROSSBIN)/ar
CFLAGS += -Driscos -mpoke-function-name -I$(GCCSDK_INSTALL_ENV)/include
LIBS = -L$(GCCSDK_INSTALL_ENV)/lib
EXEEXT ?= ,ff8
PREFIX = $(GCCSDK_INSTALL_ENV)
else
CFLAGS += -g
LIBS =
PREFIX = /usr/local
endif

ifeq ($(TARGET),)
OBJDIR = objects
LIBDIR = lib
BINDIR = bin
else
OBJDIR = $(TARGET)-objects
LIBDIR = $(TARGET)-lib
BINDIR = $(TARGET)-bin
endif

OBJS = $(addprefix $(OBJDIR)/, $(SOURCE:.c=.o))

.PHONY: all clean docs install uninstall

all: $(LIBDIR)/libnsbmp.a $(BINDIR)/decode_bmp$(EXEEXT) $(BINDIR)/decode_ico$(EXEEXT)

$(LIBDIR)/libnsbmp.a: $(OBJS) $(LIBDIR)/libnsbmp.pc
	@echo "    LINK:" $@
	@mkdir -p $(LIBDIR)
	@$(AR) $(ARFLAGS) $@ $(OBJS)

$(LIBDIR)/libnsbmp.pc: libnsbmp.pc.in
	@echo "     SED:" $@
	@mkdir -p $(LIBDIR)
	@$(SED) -e 's#PREFIX#$(PREFIX)#' $^ > $@

$(BINDIR)/decode_bmp$(EXEEXT): examples/decode_bmp.c $(LIBDIR)/libnsbmp.a
	@echo "    LINK:" $@
	@mkdir -p $(BINDIR)
	@$(CC) $(CFLAGS) -I. -o $@ $^

$(BINDIR)/decode_ico$(EXEEXT): examples/decode_ico.c $(LIBDIR)/libnsbmp.a
	@echo "    LINK:" $@
	@mkdir -p $(BINDIR)
	@$(CC) $(CFLAGS) -I. -o $@ $^

$(OBJDIR)/%.o: %.c $(HDRS)
	@echo " COMPILE:" $<
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c -o $@ $<

docs:
	${DOXYGEN}

install: $(LIBDIR)/libnsbmp.a $(LIBDIR)/libnsbmp.pc
	mkdir -p $(PREFIX)/lib/pkgconfig
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/include
	$(INSTALL) --mode=644 -t $(PREFIX)/lib $(LIBDIR)/libnsbmp.a
	$(INSTALL) --mode=644 -t $(PREFIX)/include libnsbmp.h
	$(INSTALL) --mode=644 -t $(PREFIX)/lib/pkgconfig $(LIBDIR)/libnsbmp.pc

uninstall:
	rm $(PREFIX)/lib/libnsbmp.a
	rm $(PREFIX)/include/libnsbmp.h
	rm $(PREFIX)/lib/pkgconfig/libnsbmp.pc

clean:
	-rm $(OBJS) $(LIBDIR)/libnsbmp.a $(LIBDIR)/libnsbmp.pc $(BINDIR)/decode_bmp$(EXEEXT) $(BINDIR)/decode_ico$(EXEEXT)
	-rm -rf doc
