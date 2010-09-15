#
# Makefile for building AlphaGen
#


# Component settings
COMPONENT := alphagen
COMPONENT_TYPE := binary
COMPONENT_VERSION := 1.0.0

# Setup the tooling
include build/makefiles/Makefile.tools

# Toolchain flags
WARNFLAGS := -Wall -W -Wundef -Wpointer-arith -Wcast-align \
	-Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes \
	-Wmissing-declarations -Wnested-externs -pedantic

# libpng and libz
ifneq ($(findstring clean,$(MAKECMDGOALS)),clean)
  CFLAGS := $(CFLAGS) -I$(PREFIX)/include
  LDFLAGS := $(LDFLAGS) -lpng -lz
endif

# Grab the core makefile
include build/makefiles/Makefile.top

# Extra installation rules
