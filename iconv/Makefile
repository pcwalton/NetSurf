# Component settings
COMPONENT := iconv
COMPONENT_VERSION := 0.10.0
# Default to a static library
COMPONENT_TYPE ?= lib-static

# Setup the tooling
include build/makefiles/Makefile.tools

TESTRUNNER := $(PERL) build/testtools/testrunner.pl

ifneq ($(HOST),riscos)
  export UNICODE_DIR := $(CURDIR)/riscos/!Boot/Resources/!Unicode/
endif

# Toolchain flags
WARNFLAGS := -Wall -Wextra -Wundef -Wpointer-arith -Wcast-align \
	-Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes \
	-Wmissing-declarations -Wnested-externs -Werror -pedantic
CFLAGS := -std=c99 -D_BSD_SOURCE -I$(CURDIR)/include/ \
	-I$(CURDIR)/src $(WARNFLAGS) $(CFLAGS)

ifeq ($(TARGET),riscos)
  LDFLAGS := $(LDFLAGS) -L$(CURDIR)/unicode/libro -lunicode
else
  LDFLAGS := $(LDFLAGS) -L$(CURDIR)/unicode/lib -lunicode
endif

include build/makefiles/Makefile.top

# Extra installation rules
I := /include/iconv
INSTALL_ITEMS := $(INSTALL_ITEMS) $(I):include/iconv/iconv.h
INSTALL_ITEMS := $(INSTALL_ITEMS) /lib/pkgconfig:lib$(COMPONENT).pc.in
INSTALL_ITEMS := $(INSTALL_ITEMS) /lib:$(OUTPUT)

ifeq ($(COMPONENT_TYPE),riscos-module)
  # And the RISC OS-specific targets

  DISTCLEAN_ITEMS := $(DISTCLEAN_ITEMS) iconv.zip iconv-pkg.zip

  .PHONY: riscos-dist

  # TODO: Make this sensible. Preferably by making use of the install target.
  riscos-dist: all
	$(Q)svn export riscos riscos-dist
	$(Q)$(CP) $(CPFLAGS) riscos/!Boot/Resources/!Unicode/Files/Aliases \
			riscos-dist/!Boot/Resources/!Unicode/Files/
	$(Q)$(CP) $(CPFLAGS) $(BUILDDIR)/iconv,ffa riscos-dist/!System/310/Modules/Iconv,ffa
	$(Q)svn export doc riscos-dist/doc
	$(Q)$(RM) $(RMFLAGS) -r riscos-dist/doc/Standards
	$(Q)$(CP) $(CPFLAGS) include/iconv/iconv.h riscos-dist/stubs/
	$(Q)(cd riscos-dist ; $(GCCSDK_INSTALL_CROSSBIN)/zip -9r, ../iconv.zip *)
	$(Q)$(MV) $(MVFLAGS) riscos-dist/!Boot/Resources riscos-dist
	$(Q)$(RM) $(RMFLAGS) -r riscos-dist/!Boot
	$(Q)$(MV) $(MVFLAGS) riscos-dist/!System riscos-dist/System
	$(Q)$(RM) $(RMFLAGS) -r riscos-dist/doc riscos-dist/stubs 
	$(Q)$(RM) $(RMFLAGS) riscos-dist/ReadMe
	$(Q)svn export riscpkg/RiscPkg riscos-dist/RiscPkg
	$(Q)$(CP) $(CPFLAGS) COPYING riscos-dist/RiscPkg/Copyright
	$(Q)(cd riscos-dist ; $(GCCSDK_INSTALL_CROSSBIN)/zip -9r, ../iconv-pkg.zip *)
	$(Q)$(RM) $(RMFLAGS) -r riscos-dist

endif

