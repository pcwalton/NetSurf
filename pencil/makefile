#
# This file is part of Pencil
# Licensed under the MIT License,
#                http://www.opensource.org/licenses/mit-license
# Copyright 2005 James Bursa <james@semichrome.net>
#

SOURCE = pencil_build.c pencil_save.c

GCCSDK_INSTALL_CROSSBIN ?= /home/riscos/cross/bin
GCCSDK_INSTALL_ENV ?= /home/riscos/env

CC = $(GCCSDK_INSTALL_CROSSBIN)/gcc
CFLAGS = -std=c99 -O3 -W -Wall -Wundef -Wpointer-arith -Wcast-qual \
	-Wcast-align -Wwrite-strings -Wstrict-prototypes \
	-Wmissing-prototypes -Wmissing-declarations \
	-Wnested-externs -Winline -Wno-cast-align \
	-mpoke-function-name -I$(GCCSDK_INSTALL_ENV)/include
LIBS = -L$(GCCSDK_INSTALL_ENV)/lib -lOSLib32 -lrufl
INSTALL = $(GCCSDK_INSTALL_ENV)/ro-install

all: pencil.o pencil_test,ff8

pencil.o: $(SOURCE) pencil.h pencil_internal.h
	$(CC) $(CFLAGS) -c -o $@ $(SOURCE)

pencil_test,ff8: pencil_test.c pencil.o
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

install: pencil.o
	$(INSTALL) pencil.o $(GCCSDK_INSTALL_ENV)/lib/libpencil.o
	$(INSTALL) pencil.h $(GCCSDK_INSTALL_ENV)/include/pencil.h

clean:
	-rm pencil.o pencil_test,ff8
