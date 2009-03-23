BINARIES := makerun

CFLAGS := -Wall -Wextra -Wundef -Wpointer-arith -Wcast-align \
	-Wwrite-strings -Wstrict-prototypes \
	-Wnested-externs -pedantic -std=c99 \
	-Wno-format-zero-length -Wformat-security -Wstrict-aliasing=2 \
	-Wmissing-format-attribute -Wunused -Wunreachable-code \
	-Wformat=2 -Werror-implicit-function-declaration \
	-Wmissing-declarations -Wmissing-prototypes

ECHO := echo
INSTALL := install
MKDIR := mkdir
MKDIRFLAGS := -p

# Detect host platform
HOST := $(shell uname -s)
ifeq ($(HOST),)
  # Assume RISC OS, as uname's broken there)
  HOST := riscos
else
  ifeq ($(HOST),RISC OS)
    HOST := riscos
  endif
endif

ifeq ($(HOST),riscos)
  ifeq ($(TARGET),)
    TARGET := riscos
  endif
endif

ifeq ($(TARGET),riscos)
  ifeq ($(HOST),riscos)
    GCCSDK_INSTALL_ENV := <NSLibs$$Dir>
    CC := gcc
    EXEEXT :=
    SUBTARGET :=
  else
    GCCSDK_INSTALL_CROSSBIN ?= /home/riscos/cross/bin
    GCCSDK_INSTALL_ENV ?= /home/riscos/env
    CC := $(wildcard $(GCCSDK_INSTALL_CROSSBIN)/*gcc)
    ifneq (,$(findstring arm-unknown-riscos-gcc,$(CC)))
      EXEEXT := ,e1f
      SUBTARGET := -elf-
    else
      EXEEXT := ,ff8
      SUBTARGET := -aof-
    endif
  endif

  CFLAGS += -Driscos -mpoke-function-name -I$(GCCSDK_INSTALL_ENV)/include
  LIBS = -L$(GCCSDK_INSTALL_ENV)/lib -lOSLib32
  PREFIX = $(GCCSDK_INSTALL_ENV)
else
  CFLAGS += -g
  LIBS =
  PREFIX = $(GCCSDK_INSTALL_CROSSBIN)/..
endif

-include Makefile.config

BINDIR = build-$(TARGET)$(SUBTARGET)bin

BINS = $(addprefix $(BINDIR)/, $(addsuffix $(EXEEXT), $(BINARIES)))

.PHONY: all clean install uninstall

all: $(BINS)

$(BINDIR)/%$(EXEEXT): %.c
	@$(ECHO) "   BUILD: $@"
	@$(MKDIR) $(MKDIRFLAGS) $(BINDIR)
	@$(CC) $(CFLAGS) -o $@ $< $(LIBS)

install: $(BINS)
	$(MKDIR) $(MKDIRFLAGS) $(DESTDIR)$(PREFIX)/bin
	$(INSTALL) -m 755 $(BINS) $(DESTDIR)$(PREFIX)/bin

uninstall:
	$(RM) $(addprefix $(DESTDIR)$(PREFIX)/bin/, $(addsuffix $(EXEEXT), $(BINARIES)))

clean:
	$(RM) -r $(BINDIR)

