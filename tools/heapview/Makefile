# Component settings
COMPONENT := heapview
COMPONENT_VERSION := 0.0.1
# We produce an application binary
COMPONENT_TYPE := binary

# Setup the tooling
include build/makefiles/Makefile.tools

# Toolchain flags
WARNFLAGS := -Wall -Wextra -Wundef -Wpointer-arith -Wcast-align \
	-Wwrite-strings -Werror -pedantic
CXXFLAGS := -I$(CURDIR)/include/ -I$(CURDIR)/src $(WARNFLAGS) $(CXXFLAGS) 

# GTKMM
ifneq ($(findstring clean,$(MAKECMDGOALS)),clean)
  ifneq ($(PKGCONFIG),)
    CXXFLAGS := $(CXXFLAGS) $(shell $(PKGCONFIG) gtkmm-2.4 --cflags)
    LDFLAGS := $(LDFLAGS) $(shell $(PKGCONFIG) gtkmm-2.4 --libs)
  else
    $(error Non pkg-config gtkmm support is too much effort)
  endif
endif

include build/makefiles/Makefile.top

# Extra installation rules

