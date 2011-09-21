#!/bin/amke
#
# NetSurf Source makefile for libries and browser

export TARGET ?= gtk
export PKG_CONFIG_PATH = $(TMP_PREFIX)/lib/pkgconfig
TMP_PREFIX = $(CURDIR)/prefix-$(TARGET)

#NetSurf
NETSURF_BASE = netsurf
NETSURF_VERS = 2.8
NETSURF_TARG = $(NETSURF_BASE)-$(NETSURF_VERS)

#NetSurf libraries
LIBPARSERUTILS_BASE = libparserutils
LIBPARSERUTILS_VERS = 0.1.1
LIBPARSERUTILS_TARG = $(LIBPARSERUTILS_BASE)-$(LIBPARSERUTILS_VERS)

LIBWAPCAPLET_BASE = libwapcaplet
LIBWAPCAPLET_VERS = 0.1.0
LIBWAPCAPLET_TARG = $(LIBWAPCAPLET_BASE)-$(LIBWAPCAPLET_VERS)

LIBCSS_BASE = libcss
LIBCSS_VERS = 0.1.1
LIBCSS_TARG = $(LIBCSS_BASE)-$(LIBCSS_VERS)

LIBHUBBUB_BASE = hubbub
LIBHUBBUB_VERS = 0.1.1
LIBHUBBUB_TARG = $(LIBHUBBUB_BASE)-$(LIBHUBBUB_VERS)

LIBNSBMP_BASE = libnsbmp
LIBNSBMP_VERS = 0.0.3
LIBNSBMP_TARG = $(LIBNSBMP_BASE)-$(LIBNSBMP_VERS)

LIBNSGIF_BASE = libnsgif
LIBNSGIF_VERS = 0.0.3
LIBNSGIF_TARG = $(LIBNSGIF_BASE)-$(LIBNSGIF_VERS)


#Library Target list
NSLIBTARG := $(LIBPARSERUTILS_TARG) $(LIBWAPCAPLET_TARG) $(LIBCSS_TARG) $(LIBHUBBUB_TARG) $(LIBNSBMP_TARG) $(LIBNSGIF_TARG)

.PHONY: build install clean source


build: $(TMP_PREFIX)/build-stamp

$(TMP_PREFIX)/build-stamp:
	mkdir -p $(TMP_PREFIX)/include
	mkdir -p $(TMP_PREFIX)/lib
	@for d in $(NSLIBTARG); do \
		$(MAKE) install --directory=$$d TARGET=$(TARGET) PREFIX=$(TMP_PREFIX) DESTDIR=; \
	done
	make --directory=$(NETSURF_TARG) PREFIX=$(TMP_PREFIX) TARGET=$(TARGET)
	touch $@


install: $(TMP_PREFIX)/build-stamp
	make install --directory=$(NETSURF_TARG) TARGET=$(TARGET) PREFIX=$(TMP_PREFIX) DESTDIR=$(DESTDIR)


clean:
	$(RM) $(TMP_PREFIX)
	@for d in $(NSLIBTARG); do \
		$(MAKE) distclean --directory=$$d TARGET=$(TARGET) PREFIX=$(TMP_PREFIX); \
	done
	$(MAKE) clean --directory=netsurf TARGET=$(TARGET) PREFIX=$(TMP_PREFIX)


source:$(NETSURF_TARG)-source.tar.gz

$(NETSURF_TARG)-source.tar.gz:Makefile
	$(RM) -r $(NETSURF_TARG)
	mkdir $(NETSURF_TARG)
	svn export svn://svn.netsurf-browser.org/tags/$(subst -,/,$(LIBPARSERUTILS_TARG))  $(NETSURF_TARG)/$(LIBPARSERUTILS_TARG)
	svn export svn://svn.netsurf-browser.org/tags/$(subst -,/,$(LIBWAPCAPLET_TARG))  $(NETSURF_TARG)/$(LIBWAPCAPLET_TARG)
	svn export svn://svn.netsurf-browser.org/tags/$(subst -,/,$(LIBCSS_TARG)) $(NETSURF_TARG)/$(LIBCSS_TARG)
	svn export svn://svn.netsurf-browser.org/tags/$(subst -,/,$(LIBHUBBUB_TARG)) $(NETSURF_TARG)/$(LIBHUBBUB_TARG)
	svn export svn://svn.netsurf-browser.org/tags/$(subst -,/,$(LIBNSBMP_TARG)) $(NETSURF_TARG)/$(LIBNSBMP_TARG)
	svn export svn://svn.netsurf-browser.org/tags/$(subst -,/,$(LIBNSGIF_TARG)) $(NETSURF_TARG)/$(LIBNSGIF_TARG)
	svn export svn://svn.netsurf-browser.org/tags/$(subst -,/,$(NETSURF_TARG)) $(NETSURF_TARG)/$(NETSURF_TARG)
	cp Makefile $(NETSURF_TARG)/Makefile
	tar -czf $@ $(NETSURF_TARG)
