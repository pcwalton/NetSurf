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
  CC := $(wildcard $(GCCSDK_INSTALL_CROSSBIN)/*gcc)
  AR := $(wildcard $(GCCSDK_INSTALL_CROSSBIN)/*ar)
  CFLAGS += -Driscos -mpoke-function-name -I$(GCCSDK_INSTALL_ENV)/include
  LIBS = -L$(GCCSDK_INSTALL_ENV)/lib
  ifneq (,$(findstring arm-unknown-riscos-gcc,$(CC)))
    EXEEXT := ,e1f
    SUBTARGET := -elf-
  else
    EXEEXT := ,ff8
    SUBTARGET := -aof-
  endif
  PREFIX = $(GCCSDK_INSTALL_ENV)
else
  CFLAGS += -g
  LIBS =
  PREFIX = /usr/local
endif

-include Makefile.config

OBJDIR = build-$(TARGET)$(SUBTARGET)objects
LIBDIR = build-$(TARGET)$(SUBTARGET)lib
BINDIR = build-$(TARGET)$(SUBTARGET)bin

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
	mkdir -p $(DESTDIR)$(PREFIX)/lib/pkgconfig
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	mkdir -p $(DESTDIR)$(PREFIX)/include
	$(INSTALL) -m 644 $(LIBDIR)/libnsbmp.a $(DESTDIR)$(PREFIX)/lib
	$(INSTALL) -m 644 libnsbmp.h $(DESTDIR)$(PREFIX)/include
	$(INSTALL) -m 644 $(LIBDIR)/libnsbmp.pc $(DESTDIR)$(PREFIX)/lib/pkgconfig

uninstall:
	rm $(DESTDIR)$(PREFIX)/lib/libnsbmp.a
	rm $(DESTDIR)$(PREFIX)/include/libnsbmp.h
	rm $(DESTDIR)$(PREFIX)/lib/pkgconfig/libnsbmp.pc

clean:
	-rm -rf $(OBJDIR) $(LIBDIR) $(BINDIR) doc
