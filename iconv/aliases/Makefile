makealiases_SRCS := makealiases.c

aliases_DATA := aliases-top character-sets aliases-bottom

aliases := $(CURDIR)/riscos/!Boot/Resources/!Unicode/Files/Aliases

makealiases := $(addprefix $(BUILDDIR)/$(DIR), makealiases)

DISTCLEAN_ITEMS := $(DISTCLEAN_ITEMS) $(aliases)

.PHONY: aliases

# Target for building aliases file
aliases: $(makealiases) $(addprefix $(DIR)data/, $(aliases_DATA))
	@$(makealiases) $(wordlist 2,$(words $^),$^) $(aliases)

# Target for building makealiases binary
$(makealiases): $(addprefix $(DIR), $(makealiases_SRCS))
	@$(HOST_CC) $(HOST_CCFLAGS) -o $@ $^

ifeq ($(MAKECMDGOALS),riscos-dist)
  POST_TARGETS := $(POST_TARGETS) aliases
endif

include build/makefiles/Makefile.subdir
