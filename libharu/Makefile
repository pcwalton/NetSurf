# Simple wrapper Makefile to build the libharu development snapshot we once
# made.  To update the snapshot, fetch a copy at
# http://libharu.org/files/snapshots/libharu-latest.tar.gz
#
# To build for current host:
#   $ make
#
# To build for RISC OS using cross-compiler:
#   $ make TARGET=riscos
#
# Use 'clean' rule to cleanup.

ifeq ($(TARGET),)
  TARGET = native
endif

GCCSDK_INSTALL_ENV ?= /home/riscos/env
ROOT = $(shell pwd)
PREFIX ?= $(ROOT)/prefix-$(TARGET)

SRCDIR := $(ROOT)/upstream
BUILDDIR := build-$(TARGET)

.PHONY: all gtk native riscos clean

all: $(TARGET)

gtk: native
native:
	mkdir -p $(BUILDDIR)
	cd $(BUILDDIR) && $(SRCDIR)/configure --prefix=$(PREFIX)
	cd $(BUILDDIR) && make install

riscos:
	mkdir -p $(BUILDDIR)
	cd $(BUILDDIR) && RO_CPATH=$(SRCDIR) $(GCCSDK_INSTALL_ENV)/ro-config --disable-shared --enable-static --prefix=$(PREFIX) --with-zlib=$(PREFIX) --with-png=$(PREFIX)
	cd $(BUILDDIR) && $(GCCSDK_INSTALL_ENV)/ro-make install

clean:
	-rm -rf $(BUILDDIR)
	# ro-config changes configure script
	-svn revert $(SRCDIR)/configure

