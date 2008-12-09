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

ifeq ($(TARGET),riscos)
  GCCSDK_INSTALL_CROSSBIN ?= /home/riscos/cross/bin
  GCCSDK_INSTALL_ENV ?= /home/riscos/env
  CC := $(wildcard $(GCCSDK_INSTALL_CROSSBIN)/*gcc)
  CFLAGS += -Driscos -mpoke-function-name -I$(GCCSDK_INSTALL_ENV)/include
  LIBS = -L$(GCCSDK_INSTALL_ENV)/lib
  ifneq (,$(findstring arm-unknown-riscos-gcc,$(CC)))
    EXEEXT := ,e1f
    SUBTARGET := -elf-
  else
    EXEEXT := ,ff8
    SUBTARGET := -aof-
  endif
  PREFIX = $(GCCSDK_INSTALL_ENV)
else
  CFLAGS += -g
  LIBS =
  PREFIX = /usr/local
endif

-include Makefile.config

BINDIR = $(TARGET)$(SUBTARGET)bin

BINS = $(addprefix $(BINDIR)/, $(addsuffix $(EXEEXT), $(BINARIES)))

.PHONY: all clean install uninstall

all: $(BINS)

$(BINDIR)/%$(EXEEXT): %.c
	@$(ECHO) "   BUILD: $@"
	@$(MKDIR) $(MKDIRFLAGS) $(BINDIR)
	@$(CC) $(CFLAGS) -o $@ $< 

install: $(BINS)
	$(MKDIR) $(MKDIRFLAGS) $(DESTDIR)$(PREFIX)/bin
	$(INSTALL) -m 755 $(BINS) $(DESTDIR)$(PREFIX)/bin

uninstall:
	$(RM) $(addprefix $(DESTDIR)$(PREFIX)/bin/, $(addsuffix $(EXEEXT), $(BINARIES)))

clean:
	$(RM) -r $(BINDIR)

