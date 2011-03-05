# Component settings
COMPONENT := ttf2f
COMPONENT_VERSION := 0.0.4
# We produce an application binary
COMPONENT_TYPE := binary

# Default frontend is cli
FRONTEND ?= cli

# Setup the tooling
include build/makefiles/Makefile.tools

# Toolchain flags
WARNFLAGS := -Wall -Wextra -Wundef -Wpointer-arith -Wcast-align \
	-Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes \
	-Wmissing-declarations -Wnested-externs -Werror #-pedantic
CFLAGS := $(CFLAGS) -std=c99 -D_BSD_SOURCE -D_POSIX_C_SOURCE \
	-I$(CURDIR)/include/ -I$(CURDIR)/src $(WARNFLAGS) -Dasm="__asm"

# Freetype2
ifneq ($(findstring clean,$(MAKECMDGOALS)),clean)
  ifneq ($(PKGCONFIG),)
    ifeq ($(TARGET),riscos)
      CFLAGS := $(CFLAGS) $(shell $(PKGCONFIG) freetype2 --cflags --static)
      LDFLAGS := $(LDFLAGS) $(shell $(PKGCONFIG) freetype2 --libs --static)
    else
      CFLAGS := $(CFLAGS) $(shell $(PKGCONFIG) freetype2 --cflags)
      LDFLAGS := $(LDFLAGS) $(shell $(PKGCONFIG) freetype2 --libs)
    endif
  else
    CFLAGS := $(CFLAGS) -I$(PREFIX)/include/freetype2
    LDFLAGS := $(LDFLAGS) -lfreetype -lz
  endif
endif

# OSLib (RISC OS target only)
ifneq ($(findstring clean,$(MAKECMDGOALS)),clean)
  ifeq ($(TARGET),riscos)
    CFLAGS := $(CFLAGS) -I$(PREFIX)/include -static
    LDFLAGS := $(LDFLAGS) -lOSLib32 -lOSLibSupport32 -static
  endif
endif

include build/makefiles/Makefile.top

# Extra installation rules

