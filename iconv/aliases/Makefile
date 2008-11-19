# Child makefile fragment
#
# Toolchain is provided by top-level makefile
#
# Variables provided by top-level makefile
#
# COMPONENT		The name of the component
# EXPORT		The location of the export directory
# TOP			The location of the source tree root
# RELEASEDIR		The place to put release objects
# DEBUGDIR		The place to put debug objects
#
# do_include		Canned command sequence to include a child makefile
#
# Variables provided by parent makefile:
#
# DIR			The name of the directory we're in, relative to $(TOP)
#
# Variables we can manipulate:
#
# ITEMS_CLEAN		The list of items to remove for "make clean"
# ITEMS_DISTCLEAN	The list of items to remove for "make distclean"
# TARGET_TESTS		The list of target names to run for "make test"
#
# SOURCES		The list of sources to build for $(COMPONENT)
#
# Plus anything from the toolchain

# Push parent directory onto the directory stack
sp             := $(sp).x
dirstack_$(sp) := $(d)
d              := $(DIR)

# Extend toolchain settings
CFLAGS := $(CFLAGS) -I$(TOP)/src -I$(d)

SRCS_$(d) := makealiases.c

DATA_$(d) := aliases-top character-sets aliases-bottom

ALIASES := $(addprefix $(d), Aliases)

MAKEALIASES := $(addprefix $(d), makealiases)

# Items for top-level makefile to use
ITEMS_CLEAN := $(ITEMS_CLEAN) $(MAKEALIASES)

ITEMS_DISTCLEAN := $(ITEMS_DISTCLEAN) $(ALIASES)

.PHONY: aliases

# Target for building aliases file
aliases: $(MAKEALIASES) $(addprefix $(d)data/, $(DATA_$(d)))
	@$(MAKEALIASES) $(wordlist 2,$(words $^),$^) $(ALIASES)

# Target for building makealiases binary
$(MAKEALIASES): $(addprefix $(d), $(SRCS_$(d)))
	@$(CC) $(CCFLAGS) -o $@ $^

# Now include any children we may have
MAKE_INCLUDES := $(wildcard $(d)*/Makefile)
$(eval $(foreach INC, $(MAKE_INCLUDES), $(call do_include,$(INC))))

# Finally, pop off the directory stack
d  := $(dirstack_$(sp))
sp := $(basename $(sp))
