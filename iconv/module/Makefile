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

# Module filename
MODULE := Iconv$(EXEEXT)
HEADER := $(addprefix $(d), header.o)

# Sources for module
SRCS_$(d) := module.c menu.c wrapper.c

# Objects for module
OBJS_$(d) := $(HEADER) $(addprefix $(d), $(subst .c,.o,$(SRCS_$(d))))

# Items for top-level makefile to use
ITEMS_CLEAN := $(ITEMS_CLEAN) \
	$(OBJS_$(d)) \
	$(addprefix $(d), $(subst .c,.d,$(SRCS_$(d))))

ITEMS_DISTCLEAN := $(ITEMS_DISTCLEAN) $(MODULE)

# Target for building module
# Please excuse the sick use of wordlist to get hold of the objects to link
# We can't use $(OBJS_$(d)), as $(d) will be different when the rule is run
module: release $(OBJS_$(d))
	@$(LD) -o $(MODULE) $(wordlist 2,$(words $^),$^) $(LDFLAGS) -liconv -lunicode

# Target for building module header (mildly hacky)
$(HEADER): $(addprefix $(d), header.cmhg)
ifeq ($(TARGET),riscos)
	@$(CMHG) $(CMHGFLAGS) $< -o $@ -d $(addprefix $(d), header.h)
else
	@$(TOUCH) $@
endif

DEP_$(d) :=

define dep_module
DEP_$(d) += $(2)
$(2): $(1)
	@$$(ECHO) $$(ECHOFLAGS) "DEP $(1)"
	@$$(RM) $$(RMFLAGS) $(2)
	@$$(CC) $$(DEBUGCFLAGS) -MM -MT '$(2) $(3)' -MF $(2) $(1)

endef

# Build rules for each module object
define compile_module
$(2): $(1)
	@$$(ECHO) $$(ECHOFLAGS) "==> $(1)"
	@$$(CC) -c $$(RELEASECFLAGS) -o $$@ $(1)

endef

$(eval $(foreach SOURCE,$(addprefix $(d), $(SRCS_$(d))), \
	$(call dep_module,$(SOURCE),$(SOURCE:.c=.d),$(SOURCE:.c=.o))))

ifneq ($(findstring clean,$(MAKECMDGOALS)),clean)
-include $(sort $(DEP_$(d)))
endif

$(eval $(foreach SOURCE,$(addprefix $(d), $(SRCS_$(d))), \
	$(call compile_module,$(SOURCE),$(SOURCE:.c=.o))))

# Now include any children we may have
MAKE_INCLUDES := $(wildcard $(d)*/Makefile)
$(eval $(foreach INC, $(MAKE_INCLUDES), $(call do_include,$(INC))))

# Finally, pop off the directory stack
d  := $(dirstack_$(sp))
sp := $(basename $(sp))
