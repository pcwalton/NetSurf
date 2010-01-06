# Component settings
COMPONENT := pencil
COMPONENT_VERSION := 0.0.1
# Default to a static library
COMPONENT_TYPE ?= lib-static

# Setup the tooling
include build/makefiles/Makefile.tools

TESTRUNNER := $(ECHO)

# Toolchain flags
WARNFLAGS := -Wall -W -Wundef -Wpointer-arith -Wcast-align \
	-Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes \
	-Wmissing-declarations -Wnested-externs -pedantic
# BeOS/Haiku/AmigaOS4 standard library headers create warnings
ifneq ($(TARGET),beos)
  ifneq ($(TARGET),AmigaOS)
    WARNFLAGS := $(WARNFLAGS) -Werror
  endif
endif
CFLAGS := -I$(CURDIR)/include/ -I$(CURDIR)/src $(WARNFLAGS) $(CFLAGS)
ifneq ($(GCCVER),2)
  CFLAGS := $(CFLAGS) -std=c99
else
  # __inline__ is a GCCism
  CFLAGS := $(CFLAGS) -Dinline="__inline__"
endif

# OSLib
ifneq ($(findstring clean,$(MAKECMDGOALS)),clean)
  ifeq ($(TARGET),riscos)
    CFLAGS := $(CFLAGS) -I$(PREFIX)/include
    LDFLAGS := $(LDFLAGS) -lOSLib32
  endif
endif

# RUfl
ifneq ($(findstring clean,$(MAKECMDGOALS)),clean)
  ifneq ($(PKGCONFIG),)
    CFLAGS := $(CFLAGS) $(shell $(PKGCONFIG) librufl --cflags)
    LDFLAGS := $(LDFLAGS) $(shell $(PKGCONFIG) librufl --libs)
  else
    CFLAGS := $(CFLAGS) -I$(PREFIX)/include
    LDFLAGS := $(LDFLAGS) -lrufl
  endif
endif

include build/makefiles/Makefile.top

# Extra installation rules
I := /include
INSTALL_ITEMS := $(INSTALL_ITEMS) $(I):include/pencil.h
INSTALL_ITEMS := $(INSTALL_ITEMS) /lib/pkgconfig:lib$(COMPONENT).pc.in
INSTALL_ITEMS := $(INSTALL_ITEMS) /lib:$(OUTPUT)
