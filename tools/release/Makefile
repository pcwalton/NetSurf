#!/bin/make
#
# NetSurf Source makefile for libraries and browser

export TARGET ?= gtk
export PKG_CONFIG_PATH = $(TMP_PREFIX)/lib/pkgconfig
TMP_PREFIX := $(CURDIR)/prefix-$(TARGET)

#NetSurf
NETSURF_BASE := netsurf
NETSURF_VERS := 2.8
NETSURF_TARG := $(NETSURF_BASE)-$(NETSURF_VERS)

#NetSurf libraries
LIBPARSERUTILS_BASE := libparserutils
LIBPARSERUTILS_VERS := 0.1.1
LIBPARSERUTILS_TARG := $(LIBPARSERUTILS_BASE)-$(LIBPARSERUTILS_VERS)

LIBWAPCAPLET_BASE := libwapcaplet
LIBWAPCAPLET_VERS := 0.1.0
LIBWAPCAPLET_TARG := $(LIBWAPCAPLET_BASE)-$(LIBWAPCAPLET_VERS)

LIBCSS_BASE := libcss
LIBCSS_VERS := 0.1.1
LIBCSS_TARG := $(LIBCSS_BASE)-$(LIBCSS_VERS)

LIBHUBBUB_BASE := hubbub
LIBHUBBUB_VERS := 0.1.1
LIBHUBBUB_TARG := $(LIBHUBBUB_BASE)-$(LIBHUBBUB_VERS)

LIBNSBMP_BASE := libnsbmp
LIBNSBMP_VERS := 0.0.3
LIBNSBMP_TARG := $(LIBNSBMP_BASE)-$(LIBNSBMP_VERS)

LIBNSGIF_BASE := libnsgif
LIBNSGIF_VERS := 0.0.3
LIBNSGIF_TARG := $(LIBNSGIF_BASE)-$(LIBNSGIF_VERS)

LIBNSFB_BASE := libnsfb
LIBNSFB_VERS := 0.0.1
LIBNSFB_TARG := $(LIBNSFB_BASE)-$(LIBNSFB_VERS)

LIBPENCIL_BASE := libpencil
LIBPENCIL_VERS := 0.0.1
LIBPENCIL_TARG := $(LIBPENCIL_BASE)-$(LIBPENCIL_VERS)

LIBROSPRITE_BASE := librosprite
LIBROSPRITE_VERS := 0.0.1
LIBROSPRITE_TARG := $(LIBROSPRITE_BASE)-$(LIBROSPRITE_VERS)

LIBRUFL_BASE := librufl
LIBRUFL_VERS := 0.0.1
LIBRUFL_TARG := $(LIBRUFL_BASE)-$(LIBRUFL_VERS)

LIBSVGTINY_BASE := libsvgtiny
LIBSVGTINY_VERS := 0.0.1
LIBSVGTINY_TARG := $(LIBSVGTINY_BASE)-$(LIBSVGTINY_VERS)

#Library Target list
NSLIBTARG := $(LIBPARSERUTILS_TARG) $(LIBWAPCAPLET_TARG) $(LIBCSS_TARG) \
		$(LIBHUBBUB_TARG) $(LIBNSBMP_TARG) $(LIBNSGIF_TARG)

ABLIBS := $(NSLIBTARG) $(LIBSVGTINY_TARG)

SOURCELIBS := $(ABLIBS) $(LIBNSFB_TARG) $(LIBPENCIL_TARG) \
		$(LIBROSPRITE_TARG) $(LIBRUFL_TARG)

SVNURI := svn://svn.netsurf-browser.org
ABDIR := netsurfbuild-v$(NETSURF_VERS)

define do_checkout
	svn checkout --quiet $(SVNURI)$1 $2

endef

define do_export
	svn export --quiet $(SVNURI)$1 $2

endef

define do_export_and_tar
	svn export --quiet $(SVNURI)$1 $(NETSURF_TARG)/$2
	tar -C $(NETSURF_TARG) -czf $2-src.tar.gz $2

endef

define do_clean
	$(MAKE) distclean --directory=$1 TARGET=$(TARGET)

endef

define do_prefix_install
	$(MAKE) install --directory=$1 TARGET=$(TARGET) PREFIX=$(TMP_PREFIX) DESTDIR=

endef


.PHONY: build install clean source autobuild

build: $(TMP_PREFIX)/build-stamp

$(TMP_PREFIX)/build-stamp:
	mkdir -p $(TMP_PREFIX)/include
	mkdir -p $(TMP_PREFIX)/lib
	$(foreach L,$(NSLIBTARG),$(call do_prefix_install,$(L)))
	$(MAKE) --directory=$(NETSURF_TARG) PREFIX=$(TMP_PREFIX) TARGET=$(TARGET)
	touch $@

install: $(TMP_PREFIX)/build-stamp
	$(MAKE) install --directory=$(NETSURF_TARG) TARGET=$(TARGET) PREFIX= DESTDIR=$(DESTDIR)

clean:
	$(RM) -r $(TMP_PREFIX)
	$(foreach L,$(NSLIBTARG),$(call do_clean,$(L)))
	$(MAKE) clean --directory=$(NETSURF_TARG) TARGET=$(TARGET)

source: $(NETSURF_TARG)-full-src.tar.gz
$(NETSURF_TARG)-full-src.tar.gz: Makefile
	$(RM) -r $(NETSURF_TARG)
	mkdir $(NETSURF_TARG)
	cp Makefile $(NETSURF_TARG)/Makefile
	$(foreach L,$(SOURCELIBS) $(NETSURF_TARG), \
		$(call do_export_and_tar,/tags/$(subst -,/,$(L)),$(L)))
	tar -czf $@ $(NETSURF_TARG)

autobuild:
	$(call do_export,/trunk/netsurfbuild,$(ABDIR))
	mkdir -p $(ABDIR)/downloads/releases
	$(call do_checkout,/trunk/netsurfweb,$(ABDIR)/netsurfweb)
	$(foreach L,$(ABLIBS) $(NETSURF_TARG), \
		$(call do_checkout,/tags/$(subst -,/,$(L)),$(ABDIR)/$(firstword $(subst -, ,$(L)))))

