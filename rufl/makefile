#
# This file is part of RUfl
# Licensed under the MIT License,
#                http://www.opensource.org/licenses/mit-license
# Copyright 2005 James Bursa <james@semichrome.net>
#

# choose one of the two below
COMPILER = gcc
#COMPILER = norcroft


SOURCE = rufl_init.c rufl_quit.c rufl_dump_state.c \
	rufl_character_set_test.c \
	rufl_paint.c rufl_glyph_map.c rufl_invalidate_cache.c \
	rufl_find.c rufl_decompose.c rufl_metrics.c


ifeq ($(COMPILER), gcc)
# cross-compiling using GCCSDK
GCCSDK_INSTALL_CROSSBIN ?= /home/riscos/cross/bin
GCCSDK_INSTALL_ENV ?= /home/riscos/env

CC = $(GCCSDK_INSTALL_CROSSBIN)/gcc
CFLAGS = -std=c99 -O3 -W -Wall -Wundef -Wpointer-arith -Wcast-qual \
	-Wcast-align -Wwrite-strings -Wstrict-prototypes \
	-Wmissing-prototypes -Wmissing-declarations \
	-Wnested-externs -Winline -Wno-unused-parameter \
	-mpoke-function-name -I$(GCCSDK_INSTALL_ENV)/include
LIBS = -L$(GCCSDK_INSTALL_ENV)/lib -lOSLib32
INSTALL = $(GCCSDK_INSTALL_ENV)/ro-install

all: rufl.o rufl_test,ff8 rufl_chars,ff8

rufl.o: $(SOURCE) rufl.h rufl_internal.h Glyphs
	$(CC) $(CFLAGS) -c -o $@ $(SOURCE)

install: rufl.o
	$(INSTALL) rufl.o $(GCCSDK_INSTALL_ENV)/lib/librufl.o
	$(INSTALL) rufl.h $(GCCSDK_INSTALL_ENV)/include/rufl.h
else
# compiling on RISC OS using Norcroft
CC = cc
CFLAGS = -fn -ecz -wap -IOSLib: -DNDEBUG
LD = link
LDFLAGS = -aof
LIBS = OSLib:o.oslib32
MKDLK = makedlk
SOURCE += strfuncs.c
OBJS = $(SOURCE:.c=.o)

all: rufl.o rufl/pyd rufl_test,ff8 rufl_chars,ff8
rufl.o: o.rufl
o.rufl: $(OBJS) rufl.h rufl_internal.h Glyphs
	$(LD) $(LDFLAGS) -o $@ $(OBJS)
o.ruflmodule: ruflmodule.o rufl.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
ruflmodule.o: ruflmodule.c
	$(CC) -fn -wp -IPyInc:Include,PyInc:RISCOS,TCPIPLibs:,OSLib: -c $@ $<
rufl/pyd: o.ruflmodule
	$(MKDLK) -s <Python$$Dir>.RISCOS.s.linktab -o $< -d $@ -e initrufl
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

endif


# common rules
rufl_glyph_map.c: Glyphs makeglyphs
	./makeglyphs < Glyphs > $@

rufl_test,ff8: rufl_test.c rufl.o
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

rufl_chars,ff8: rufl_chars.c rufl.o
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^


clean:
	-rm rufl.o rufl_glyph_map.c rufl_test,ff8 rufl_chars,ff8
