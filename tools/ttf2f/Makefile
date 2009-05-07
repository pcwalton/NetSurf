# Component settings
COMPONENT := ttf2f
COMPONENT_VERSION := 0.0.3
# We produce an application binary
COMPONENT_TYPE := binary

# Setup the tooling
include build/makefiles/Makefile.tools

# Toolchain flags
WARNFLAGS := -Wall -Wextra -Wundef -Wpointer-arith -Wcast-align \
	-Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes \
	-Wmissing-declarations -Wnested-externs -Werror -pedantic
CFLAGS := $(CFLAGS) -std=c99 -D_BSD_SOURCE -I$(CURDIR)/include/ \
	-I$(CURDIR)/src $(WARNFLAGS) -Dasm="__asm"

# Freetype2
ifneq ($(findstring clean,$(MAKECMDGOALS)),clean)
  ifneq ($(PKGCONFIG),)
    CFLAGS := $(CFLAGS) $(shell $(PKGCONFIG) freetype2 --cflags)
    LDFLAGS := $(LDFLAGS) $(shell $(PKGCONFIG) freetype2 --libs)
  else
    CFLAGS := $(CFLAGS) -I$(PREFIX)/include/freetype2
    LDFLAGS := $(LDFLAGS) -lfreetype -lz
  endif
endif

include build/makefiles/Makefile.top

# Extra installation rules

