makealiases_SRCS := makealiases.c

aliases_DATA := aliases-top character-sets aliases-bottom

aliases := $(CURDIR)/riscos/!Boot/Resources/!Unicode/Files/Aliases

makealiases := $(addprefix $(BUILDDIR)/, \
		$(subst /,_,$(addprefix $(DIR),makealiases)))

DISTCLEAN_ITEMS := $(DISTCLEAN_ITEMS) $(aliases)

# Target for building aliases file
$(aliases): $(makealiases) $(addprefix $(DIR)data/, $(aliases_DATA))
	$(VQ)$(ECHO) $(ECHOFLAGS) " ALIASES: $@"
	$(Q)$(makealiases) $(wordlist 2,$(words $^),$^) $@

# Target for building makealiases binary
$(makealiases): $(addprefix $(DIR), $(makealiases_SRCS))
	$(Q)$(HOST_CC) $(HOST_CFLAGS) -o $@ $^

ifeq ($(MAKECMDGOALS),riscos-dist)
  POST_TARGETS := $(POST_TARGETS) $(aliases)
endif

include build/makefiles/Makefile.subdir
