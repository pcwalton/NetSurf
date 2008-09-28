#
# This file is part of Pencil
# Licensed under the MIT License,
#                http://www.opensource.org/licenses/mit-license
# Copyright 2005 James Bursa <james@semichrome.net>
#

SOURCE = pencil_build.c pencil_save.c
HDRS =  pencil.h pencil_internal.h

GCCSDK_INSTALL_CROSSBIN ?= /home/riscos/cross/bin
GCCSDK_INSTALL_ENV ?= /home/riscos/env

.PHONY: all install clean

CC := $(wildcard $(GCCSDK_INSTALL_CROSSBIN)/*gcc)
AR := $(wildcard $(GCCSDK_INSTALL_CROSSBIN)/*ar)
CFLAGS = -std=c99 -O3 -W -Wall -Wundef -Wpointer-arith -Wcast-qual \
	-Wcast-align -Wwrite-strings -Wstrict-prototypes \
	-Wmissing-prototypes -Wmissing-declarations \
	-Wnested-externs -Winline -Wno-cast-align \
	-mpoke-function-name -I$(GCCSDK_INSTALL_ENV)/include
ARFLAGS = cr
LIBS = -L$(GCCSDK_INSTALL_ENV)/lib -lOSLib32 -lrufl
INSTALL = $(GCCSDK_INSTALL_ENV)/ro-install
ifneq (,$(findstring arm-unknown-riscos-gcc,$(CC)))
  EXEEXT=,e1f
else
  EXEEXT=,ff8
endif

OBJS = $(SOURCE:.c=.o)

all: libpencil.a pencil_test$(EXEEXT)

libpencil.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

pencil_test$(EXEEXT): pencil_test.c libpencil.a
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

install: libpencil.a
	$(INSTALL) libpencil.a $(GCCSDK_INSTALL_ENV)/lib/libpencil.a
	$(INSTALL) pencil.h $(GCCSDK_INSTALL_ENV)/include/pencil.h

clean:
	-rm *.o libpencil.a pencil_test$(EXEEXT)

.c.o: $(HDRS)
	$(CC) $(CFLAGS) -c -o $@ $<
