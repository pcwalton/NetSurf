# Simple wrapper Makefile to build the libharu development snapshot we made.
# To update snapshot, use http://libharu.org/files/snapshots/libharu-latest.tar.gz

GCCSDK_INSTALL_ENV ?= /home/riscos/env
ROOT = $(shell pwd)
PREFIX ?= $(ROOT)/prefix-$(TARGET)

.PHONY: gtk riscos

gtk: TARGET=gtk
gtk:
	cd upstream && ./configure --prefix=$(PREFIX) --with-zlib=$(PREFIX) --with-png=$(PREFIX)
	cd upstream && make install

riscos: TARGET=riscos
riscos:
	cd upstream && $(GCCSDK_INSTALL_ENV)/ro-config --disable-shared --enable-static --prefix=$(PREFIX) --with-zlib=$(PREFIX) --with-png=$(PREFIX)
	cd upstream && $(GCCSDK_INSTALL_ENV)/ro-make install

