# Component settings
COMPONENT := nspng
COMPONENT_VERSION := 0.0.1
# Default to a static library
COMPONENT_TYPE ?= lib-static

# Setup the tooling
include build/makefiles/Makefile.tools

# Toolchain flags
WARNFLAGS := -Wall -Wundef -Wpointer-arith -Wcast-align \
	-Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes \
	-Wmissing-declarations -Wnested-externs -Werror -pedantic
ifneq ($(GCCVER),2)
  WARNFLAGS := $(WARNFLAGS) -Wextra
endif
CFLAGS := $(CFLAGS) -D_BSD_SOURCE -I$(CURDIR)/include/ \
	-I$(CURDIR)/src $(WARNFLAGS) 
ifneq ($(GCCVER),2)
  CFLAGS := $(CFLAGS) -std=c99
else
  # __inline__ is a GCCism
  CFLAGS := $(CFLAGS) -Dinline="__inline__"
endif

# Zlib
ifneq ($(findstring clean,$(MAKECMDGOALS)),clean)
  CFLAGS := $(CFLAGS) -I$(PREFIX)/include
  LDFLAGS := $(LDFLAGS) -lz
endif

include build/makefiles/Makefile.top

# Add SDL to test flags
ifeq ($(WANT_TEST),yes)
  ifneq ($(PKGCONFIG),)
    TESTCFLAGS := $(TESTCFLAGS) \
		$(shell $(PKGCONFIG) $(PKGCONFIGFLAGS) --cflags sdl)
    TESTLDFLAGS := $(TESTLDFLAGS) \
		$(shell $(PKGCONFIG) $(PKGCONFIGFLAGS) --libs sdl)
  else
    TESTCFLAGS := $(TESTCFLAGS) -I$(PREFIX)/include/SDL
    TESTLDFLAGS := $(TESTLDFLAGS) -lSDL
  endif
endif

# Extra installation rules
I := /include
INSTALL_ITEMS := $(INSTALL_ITEMS) $(I):include/libnspng.h
INSTALL_ITEMS := $(INSTALL_ITEMS) /lib/pkgconfig:lib$(COMPONENT).pc.in
INSTALL_ITEMS := $(INSTALL_ITEMS) /lib:$(OUTPUT)
