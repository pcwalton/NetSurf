#
# Makefile for NetSurf and required libraries
#

usage:
	@echo Please use one of the following targets:
	@echo
	@echo "  make beos             NetSurf for BeOS and Haiku"
	@echo "  make cocoa            NetSurf for OSX with the Cocoa interface"
	@echo "  make gtk              NetSurf with GTK interface"
	@echo "  make riscos           NetSurf for RISC OS"
	@echo "  make <target> netsurf NetSurf only <target>, without the libraries"
	@echo "  make <target> clean  Clean the build for <target>"
	@echo
	@echo "Optionally append PREFIX=..."

export ROOT = $(shell pwd)
export PKG_CONFIG_PATH = $(PREFIX)/lib/pkgconfig
export PREFIX ?= $(ROOT)/prefix-$(TARGET)

NSLIBS := libparserutils hubbub libnsbmp libnsgif libsvgtiny libwapcaplet libcss libdom


ifneq ($(filter clean,$(MAKECMDGOALS)),)
else
ifneq  ($(filter netsurf,$(MAKECMDGOALS)),)
else
$(MAKECMDGOALS): full
endif
endif


ifneq ($(filter beos,$(MAKECMDGOALS)),)
export TARGET=beos
export TARGETNAME=BeOS
export PKG_CONFIG_PATH=
export GCCVER=2
endif

ifneq ($(filter cocoa,$(MAKECMDGOALS)),)
export TARGET=cocoa
export TARGETNAME=Darwin
export PKG_CONFIG_PATH=
endif

ifneq ($(filter gtk,$(MAKECMDGOALS)),)
export TARGET=gtk
export TARGETNAME=GTK interface
export PKG_CONFIG_PATH=
endif

ifneq ($(filter riscos,$(MAKECMDGOALS)),)
export TARGET=riscos
export TARGETNAME=RISC OS
export PKG_CONFIG_PATH=
NSLIBS += pencil rufl tools
endif


.PHONY: clean netsurf full beos cocoa gtk riscos

# avoid "nothing to be done for..."
beos cocoa gtk riscos:
	@true


clean:
	@echo -----------------------------------------------------------------
	@echo
	@echo Cleaning NetSurf for $(TARGETNAME)...
	@echo
	@echo -----------------------------------------------------------------
	@echo
	@for d in $(NSLIBS); do \
		echo Cleaning $$d...; \
		$(MAKE) clean --directory=$$d TARGET=$(TARGET) PREFIX=$(PREFIX) || exit $?; \
	done
	$(MAKE) clean --directory=netsurf TARGET=$(TARGET) PREFIX=$(PREFIX)

netsurf:
	@echo -----------------------------------------------------------------
	@echo
	@echo Building NetSurf only for $(TARGETNAME) with the following options:
	@echo
	@echo TARGET = $(TARGET)
	@echo PREFIX = $(PREFIX)
	@echo PKG_CONFIG_PATH = $(PKG_CONFIG_PATH)
	@echo
	@echo -----------------------------------------------------------------
	@echo
	$(MAKE) --directory=netsurf TARGET=$(TARGET) PREFIX=$(PREFIX)

full:
	@echo -----------------------------------------------------------------
	@echo
	@echo Building NetSurf for $(TARGETNAME) with the following options:
	@echo
	@echo TARGET = $(TARGET)
	@echo PREFIX = $(PREFIX)
	@echo PKG_CONFIG_PATH = $(PKG_CONFIG_PATH)
	@echo
	@echo -----------------------------------------------------------------
	@echo
	mkdir -p $(PREFIX)/include
	mkdir -p $(PREFIX)/lib
	for d in $(NSLIBS); do \
		echo Installing $$d...; \
		$(MAKE) install --directory=$$d TARGET=$(TARGET) PREFIX=$(PREFIX) || exit $?; \
	done
	$(MAKE) --directory=netsurf TARGET=$(TARGET) PREFIX=$(PREFIX)

