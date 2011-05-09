#
# Makefile for NetSurf and required libraries
#

usage:
	@echo Please use one of the following targets:
	@echo
	@echo "  make beos      NetSurf for BeOS and Haiku"
	@echo "  make gtk       NetSurf with GTK interface"
	@echo "  make riscos    NetSurf for RISC OS"
	@echo
	@echo Optionally append PREFIX=...

export ROOT = $(shell pwd)
export PKG_CONFIG_PATH = $(PREFIX)/lib/pkgconfig
export PREFIX ?= $(ROOT)/prefix-$(TARGET)

ifneq ($(filter clean,$(MAKECMDGOALS)),)
LIBGOAL := clean
NSGOAL  := clean
endif
LIBGOAL ?= install

.PHONY: clean

clean:

beos: export TARGET=beos
beos: export PKG_CONFIG_PATH=
beos: export GCCVER=2
beos clean-beos:
	echo $(LIBGOAL)
	#exit 1
	@echo -----------------------------------------------------------------
	@echo
	@echo Building NetSurf for BeOS with the following options:
	@echo
	@echo TARGET = $(TARGET)
	@echo PREFIX = $(PREFIX)
	@echo PKG_CONFIG_PATH = $(PKG_CONFIG_PATH)
	@echo
	@echo -----------------------------------------------------------------
	@echo
	mkdir -p $(PREFIX)/include
	mkdir -p $(PREFIX)/lib
	$(MAKE) $(LIBGOAL) --directory=libparserutils TARGET=$(TARGET) PREFIX=$(PREFIX)
	$(MAKE) $(LIBGOAL) --directory=hubbub TARGET=$(TARGET) PREFIX=$(PREFIX)
	$(MAKE) $(LIBGOAL) --directory=libnsbmp TARGET=$(TARGET) PREFIX=$(PREFIX)
	$(MAKE) $(LIBGOAL) --directory=libnsgif TARGET=$(TARGET) PREFIX=$(PREFIX)
	$(MAKE) $(LIBGOAL) --directory=libsvgtiny TARGET=$(TARGET) PREFIX=$(PREFIX)
	$(MAKE) $(LIBGOAL) --directory=libwapcaplet TARGET=$(TARGET) PREFIX=$(PREFIX)
	$(MAKE) $(LIBGOAL) --directory=libcss TARGET=$(TARGET) PREFIX=$(PREFIX)
	$(MAKE) $(NSGOAL) --directory=netsurf TARGET=$(TARGET) PREFIX=$(PREFIX)

cocoa: export TARGET=cocoa
cocoa:
	@echo -----------------------------------------------------------------
	@echo
	@echo Building NetSurf for Darwin with the following options:
	@echo
	@echo TARGET = $(TARGET)
	@echo PREFIX = $(PREFIX)
	@echo PKG_CONFIG_PATH = $(PKG_CONFIG_PATH)
	@echo
	@echo -----------------------------------------------------------------
	@echo
	mkdir -p $(PREFIX)/include
	mkdir -p $(PREFIX)/lib
	$(MAKE) $(LIBGOAL) --directory=libparserutils TARGET=$(TARGET) PREFIX=$(PREFIX)
	$(MAKE) $(LIBGOAL) --directory=hubbub TARGET=$(TARGET) PREFIX=$(PREFIX)
	$(MAKE) $(LIBGOAL) --directory=libnsbmp TARGET=$(TARGET) PREFIX=$(PREFIX)
	$(MAKE) $(LIBGOAL) --directory=libnsgif TARGET=$(TARGET) PREFIX=$(PREFIX)
	$(MAKE) $(LIBGOAL) --directory=libsvgtiny TARGET=$(TARGET) PREFIX=$(PREFIX)
	$(MAKE) $(LIBGOAL) --directory=libwapcaplet TARGET=$(TARGET) PREFIX=$(PREFIX)
	$(MAKE) $(LIBGOAL) --directory=libcss TARGET=$(TARGET) PREFIX=$(PREFIX)
	$(MAKE) $(NSGOAL) --directory=netsurf TARGET=$(TARGET) PREFIX=$(PREFIX)

gtk: export TARGET=gtk
gtk:
	@echo -----------------------------------------------------------------
	@echo
	@echo Building NetSurf with GTK interface with the following options:
	@echo
	@echo TARGET = $(TARGET)
	@echo PREFIX = $(PREFIX)
	@echo PKG_CONFIG_PATH = $(PKG_CONFIG_PATH)
	@echo
	@echo -----------------------------------------------------------------
	@echo
	mkdir -p $(PREFIX)/include
	mkdir -p $(PREFIX)/lib
	make $(LIBGOAL) --directory=libparserutils TARGET=$(TARGET) PREFIX=$(PREFIX)
	make $(LIBGOAL) --directory=hubbub TARGET=$(TARGET) PREFIX=$(PREFIX)
	make $(LIBGOAL) --directory=libnsbmp TARGET=$(TARGET) PREFIX=$(PREFIX)
	make $(LIBGOAL) --directory=libnsgif TARGET=$(TARGET) PREFIX=$(PREFIX)
	make $(LIBGOAL) --directory=libwapcaplet TARGET=$(TARGET) PREFIX=$(PREFIX)
	make $(LIBGOAL) --directory=libcss TARGET=$(TARGET) PREFIX=$(PREFIX)
	make $(NSGOAL) --directory=netsurf TARGET=$(TARGET) PREFIX=$(PREFIX)

riscos: export TARGET=riscos
riscos:
	@echo -----------------------------------------------------------------
	@echo
	@echo Building NetSurf for RISC OS with the following options:
	@echo
	@echo TARGET = $(TARGET)
	@echo PREFIX = $(PREFIX)
	@echo PKG_CONFIG_PATH = $(PKG_CONFIG_PATH)
	@echo
	@echo -----------------------------------------------------------------
	@echo
	mkdir -p $(PREFIX)/include
	mkdir -p $(PREFIX)/lib
	make $(LIBGOAL) --directory=libparserutils --makefile=Makefile-riscos TARGET=$(TARGET) PREFIX=$(PREFIX)
	make $(LIBGOAL) --directory=hubbub --makefile=Makefile-riscos TARGET=$(TARGET) PREFIX=$(PREFIX)
	make $(LIBGOAL) --directory=libnsbmp TARGET=$(TARGET) PREFIX=$(PREFIX)
	make $(LIBGOAL) --directory=libnsgif TARGET=$(TARGET) PREFIX=$(PREFIX)
	make $(LIBGOAL) --directory=libsvgtiny TARGET=$(TARGET) PREFIX=$(PREFIX)
	make $(LIBGOAL) --directory=libwapcaplet TARGET=$(TARGET) PREFIX=$(PREFIX)
	make $(LIBGOAL) --directory=libcss TARGET=$(TARGET) PREFIX=$(PREFIX)
	make $(LIBGOAL) --directory=pencil TARGET=$(TARGET) PREFIX=$(PREFIX)
	make $(LIBGOAL) --directory=rufl TARGET=$(TARGET) PREFIX=$(PREFIX)
	make $(LIBGOAL) --directory=tools PREFIX=$(PREFIX)
	make $(NSGOAL) --directory=netsurf TARGET=$(TARGET) PREFIX=$(PREFIX)

