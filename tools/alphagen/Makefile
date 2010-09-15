# Makefile for building AlphaGen on RISC OS.

# Needs NSTools for libpng and libz.
#     http://www.netsurf-browser.org/downloads/other/nstools.zip

.SUFFIXES: .c .o
.c.o:
	gcc -I<NSLibs$$Dir>/include -mpoke-function-name -mthrowback -c -O2 $<

SRC = $(wildcard *.c)

OBJ = $(SRC:.c=.o)

nslog: $(OBJ)
	gcc -L<NSLibs$$Dir>/lib -lpng -lz -o alphagen $(OBJ)
