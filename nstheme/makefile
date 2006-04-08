#
# This file is part of NetSurf, http://netsurf.sourceforge.net/
# Licensed under the GNU General Public License,
#                http://www.opensource.org/licenses/gpl-license
#

# There is 1 possible build of NSTheme:
#
#   riscos -- standard RISC OS build
#
# "riscos" can be compiled under RISC OS, or cross-compiled using gccsdk.

OBJECTS_COMMON = messages.o utils.o				# utils/
OBJECTS_COMMON += nstheme.o options.o				# desktop/

OBJECTS_RISCOS = $(OBJECTS_COMMON)
OBJECTS_RISCOS += dialog.o gui.o help.o menus.o wimp.o save.o


OBJDIR_RISCOS = $(shell $(CC) -dumpmachine)
SOURCES_RISCOS=$(OBJECTS_RISCOS:.o=.c)
OBJS_RISCOS=$(OBJECTS_RISCOS:%.o=$(OBJDIR_RISCOS)/%.o)

# Inclusion of platform specific files has to occur after the OBJDIR stuff as
# that is refered to in the files

OS = $(word 2,$(subst -, ,$(shell gcc -dumpmachine)))
ifeq ($(OS),riscos)
include riscos.mk
else
include posix.mk
endif

VPATH = desktop:riscos:utils

WARNFLAGS = -W -Wall -Wundef -Wpointer-arith -Wcast-qual \
	-Wcast-align -Wwrite-strings -Wstrict-prototypes \
	-Wmissing-prototypes -Wmissing-declarations -Wredundant-decls \
	-Wnested-externs -Winline -Wno-unused-parameter -Wuninitialized

# CFLAGS have to appear after the inclusion of platform specific files as the
# PLATFORM_CFLAGS variables are defined in them

CFLAGS_RISCOS = -std=c9x -D_BSD_SOURCE -Driscos -DBOOL_DEFINED -O2 \
	$(WARNFLAGS) -I.. $(PLATFORM_CFLAGS_RISCOS) -mpoke-function-name \

# targets
riscos: $(RUNIMAGE)
$(RUNIMAGE) : $(OBJS_RISCOS)
	$(CC) -o $@ $(LDFLAGS_RISCOS) $^

nstheme.zip: $(RUNIMAGE)
	rm nstheme.zip; riscos-zip -9vr, nstheme.zip !NSTheme

# pattern rule for c source
$(OBJDIR_RISCOS)/%.o : %.c
	@echo "==> $<"
	@$(CC) -o $@ -c $(CFLAGS_RISCOS) $<

# Generate dependencies.
# To disable automatic regeneration of dependencies (eg. if perl is not
# available), remove */*.[ch] from the line below.
# Under RISC OS, you may require *Set UnixFS$sfix "", if perl gives
# "No such file or directory" errors.
depend: */*.[ch]
	@echo "--> modified files $?"
	@echo "--> updating dependencies"
	@-mkdir -p $(OBJDIR_RISCOS)
	@perl scandeps nstheme $(OBJDIR_RISCOS) -- $^ > depend

include depend

# remove generated files
clean :
	-rm $(OBJDIR_RISCOS)/*
