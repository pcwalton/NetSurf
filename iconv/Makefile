# Component settings
COMPONENT := iconv
# Default to a static library
COMPONENT_TYPE ?= lib-static

# Setup the tooling
include build/makefiles/Makefile.tools

TESTRUNNER := $(PERL) build/testtools/testrunner.pl

# Toolchain flags
WARNFLAGS := -Wall -Wextra -Wundef -Wpointer-arith -Wcast-align \
	-Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes \
	-Wmissing-declarations -Wnested-externs -Werror -pedantic
CFLAGS := $(CFLAGS) -std=c99 -D_BSD_SOURCE -I$(CURDIR)/include/ \
	-I$(CURDIR)/src $(WARNFLAGS) 

ifeq ($(TARGET),riscos)
  LDFLAGS := $(LDFLAGS) -L$(CURDIR)/unicode/libro
else
  LDFLAGS := $(LDFLAGS) -L$(CURDIR)/unicode/lib
endif

include build/makefiles/Makefile.top

# Extra installation rules
INSTALL_ITEMS := $(INSTALL_ITEMS) /include/iconv:include/iconv/iconv.h
INSTALL_ITEMS := $(INSTALL_ITEMS) /lib/pkgconfig:lib$(COMPONENT).pc.in
INSTALL_ITEMS := $(INSTALL_ITEMS) /lib:$(BUILDDIR)/lib$(COMPONENT)$(LIBEXT)

ifeq ($(TARGET),riscos)
  # And the RISC OS-specific targets

  DISTCLEAN_ITEMS := $(DISTCLEAN_ITEMS) iconv.zip iconv-pkg.zip

  .PHONY: riscos-dist

  riscos-dist: all
	@svn export riscos riscos-dist
	@$(CP) $(CPFLAGS) riscos/!Boot/Resources/!Unicode/Files/Aliases \
			riscos-dist/!Boot/Resources/!Unicode/Files/
	@$(CP) $(CPFLAGS) Iconv,ffa riscos-dist/!System/310/Modules/
	@svn export doc riscos-dist/doc
	@$(RM) $(RMFLAGS) -r riscos-dist/doc/Standards
	@$(CP) $(CPFLAGS) include/iconv/iconv.h riscos-dist/stubs/
	@(cd riscos-dist ; $(GCCSDK_INSTALL_ENV)/bin/zip -9r, ../iconv.zip *)
	@$(MV) $(MVFLAGS) riscos-dist/!Boot/Resources riscos-dist
	@$(RM) $(RMFLAGS) -r riscos-dist/!Boot
	@$(MV) $(MVFLAGS) riscos-dist/!System riscos-dist/System
	@$(RM) $(RMFLAGS) -r riscos-dist/doc riscos-dist/stubs 
	@$(RM) $(RMFLAGS) riscos-dist/ReadMe
	@svn export riscpkg/RiscPkg riscos-dist/RiscPkg
	@$(CP) $(CPFLAGS) COPYING riscos-dist/RiscPkg/Copyright
	@(cd riscos-dist ; $(GCCSDK_INSTALL_ENV)/bin/zip -9r, ../iconv-pkg.zip *)
	@$(RM) $(RMFLAGS) -r riscos-dist

endif
