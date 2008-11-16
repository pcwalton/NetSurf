# Makefile fragment for GNU libiconv tests
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
CFLAGS := $(CFLAGS) -I$(TOP)/src/ -I$(d)

# Tests
TESTS_$(d) := table-from table-to genutf8 gengb18030z
TESTS_$(d) := $(TESTS_$(d))

# Items for top-level makefile to use
ITEMS_CLEAN := $(ITEMS_CLEAN) \
	$(addprefix $(d), $(addsuffix $(EXEEXT), $(TESTS_$(d)))) \
	$(addprefix $(d), tmp-*) \
	$(addprefix $(d), UTF-8.TXT) \
	$(addprefix $(d), GB18030.TXT) \
	$(addprefix $(d), $(addsuffix .gcda, $(TESTS_$(d)))) \
	$(addprefix $(d), $(addsuffix .gcno, $(TESTS_$(d)))) \
	$(addprefix $(d), $(addsuffix .d, $(TESTS_$(d))))

# Targets for top-level makefile to run
TARGET_TESTS := $(TARGET_TESTS) test_$(d)

# Now we get to hack around so that we know what directory we're in.
# $(d) no longer exists when running the commands for a target, so we can't
# simply use it verbatim. Assigning to a variable doesn't really help, as
# there's no guarantee that someone else hasn't overridden that variable.
# So, what we do is make the target depend on $(d), then pick it out of the
# dependency list when running commands. This isn't pretty, but is effective.
test_$(d): $(d) $(addprefix $(d), $(TESTS_$(d)))
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< ASCII
#	/* General multi-byte encodings */
#	$(TOP)/$<genutf8 > $(TOP)/$<UTF-8.TXT && $(TOP)/$<check-stateless $(TOP) $(TOP)/$< UTF-8
#	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< UCS-2BE
#	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< UCS-2LE
#	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< UCS-4BE
#	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< UCS-4LE
	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< UTF-16
	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< UTF-16BE
	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< UTF-16LE
	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< UTF-32
	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< UTF-32BE
	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< UTF-32LE
	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< UTF-7
#	/* 8-bit encodings */
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< ISO-8859-1
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< ISO-8859-2
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< ISO-8859-3
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< ISO-8859-4
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< ISO-8859-5
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< ISO-8859-6
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< ISO-8859-7
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< ISO-8859-8
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< ISO-8859-9
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< ISO-8859-10
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< ISO-8859-11
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< ISO-8859-13
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< ISO-8859-14
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< ISO-8859-15
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< ISO-8859-16
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< KOI8-R
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< KOI8-U
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< KOI8-RU
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP1250
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP1251
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP1252
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP1253
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP1254
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP1255
#	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< CP1255
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP1256
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP1257
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP1258
#	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< CP1258
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP850
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP862
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP866
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< MacRoman
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< MacCentralEurope
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< MacIceland
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< MacCroatian
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< MacRomania
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< MacCyrillic
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< MacUkraine
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< MacGreek
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< MacTurkish
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< MacHebrew
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< MacArabic
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< MacThai
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< HP-ROMAN8
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< NEXTSTEP
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< ARMSCII-8
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< Georgian-Academy
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< Georgian-PS
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< KOI8-T
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< PT154
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< RK1048
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< MuleLao-1
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP1133
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< TIS-620
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP874
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< VISCII
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< TCVN
#	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< TCVN
#	/* CJK character sets */
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< ISO646-JP
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< JIS_X0201
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< JIS_X0208 # redundant, see EUC-JP
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< JIS_X0212 # redundant, see EUC-JP
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< ISO646-CN
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< GB_2312-80 # redundant, see EUC-CN
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< ISO-IR-165
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< KSC_5601 # redundant, see EUC-KR
#	/* CJK encodings */
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< EUC-JP
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< SHIFT_JIS
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP932
	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< ISO-2022-JP
	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< ISO-2022-JP-2
	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< ISO-2022-JP-1
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< EUC-CN
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< GBK
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP936
	{ cat $(TOP)/$<GB18030-BMP.TXT ; $(TOP)/$<gengb18030z ; } | sort > $(TOP)/$<GB18030.TXT && $(TOP)/$<check-stateless $(TOP) $(TOP)/$< GB18030
	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< ISO-2022-CN
	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< ISO-2022-CN-EXT
	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< HZ
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< EUC-TW
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< BIG5
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP950
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< BIG5-HKSCS:1999
	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< BIG5-HKSCS:1999
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< BIG5-HKSCS:2001
	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< BIG5-HKSCS:2001
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< BIG5-HKSCS:2004
	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< BIG5-HKSCS:2004
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< EUC-KR
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP949
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< JOHAB
	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< ISO-2022-KR
#	/* transliteration */
	$(TOP)/$<check-translit $(TOP)/$< Quotes UTF-8 ISO-8859-1
	$(TOP)/$<check-translit $(TOP)/$< Quotes UTF-8 ASCII
	$(TOP)/$<check-translit $(TOP)/$< Translit1 ISO-8859-1 ASCII
	$(TOP)/$<check-translitfailure $(TOP)/$< TranslitFail1 ISO-8859-1 ASCII
#	/* substitution */
	$(TOP)/$<check-subst
#	/* system dependent and extra encodings */
#	/* AIX specific encodings */
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP856
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP922
#	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP943
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP1046
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP1124
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP1129
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP1161
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP1162
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP1163
#	/* OSF/1 specific encodings */
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< DEC-KANJI
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< DEC-HANYU
#	/* DOS specific encodings */
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP437
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP737
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP775
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP852
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP853
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP855
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP857
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP858
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP860
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP861
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP863
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP864
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP865
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP869
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< CP1125
#	/* Extra encodings */
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< EUC-JISX0213
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< SHIFT_JISX0213
	$(TOP)/$<check-stateful $(TOP) $(TOP)/$< ISO-2022-JP-3
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< BIG5-2003
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< TDS565
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< ATARIST
	$(TOP)/$<check-stateless $(TOP) $(TOP)/$< RISCOS-LATIN1

DEP_$(d) :=

define dep_test
DEP_$(d) += $(2)
$(2): $(1)
	@$$(RM) $$(RMFLAGS) $(2)
	@$$(CC) $$(DEBUGCFLAGS) -MM -MT '$(2) $(3)' -MF $(2) $(1)

endef

# Build rules for each test binary -- they all depend on the debug library
# Except when building on RISC OS, of course because then make utterly fails to
# detect that the debug library exists.
define compile_test
ifeq ($(HOST),riscos)
$(2): $(1)
else
$(2): $$(TOP)/$$(COMPONENT)-debug.a $(1)
endif
	@$$(ECHO) $$(ECHOFLAGS) "==> $(1)"
	@$$(CC) -c -g $$(DEBUGCFLAGS) -o $$@.o $(1)
	@$$(LD) -g -o $$@ $$@.o $$(LDFLAGS) -liconv-debug -lunicode
	@$$(RM) $$(RMFLAGS) $$@.o

endef

$(eval $(foreach TEST,$(addprefix $(d), $(TESTS_$(d))), \
	$(call dep_test,$(addsuffix .c, $(TEST)),$(addsuffix .d, $(TEST)),$(TEST))))

ifneq ($(findstring clean,$(MAKECMDGOALS)),clean)
-include $(sort $(DEP_$(d)))
endif

$(eval $(foreach TEST,$(addprefix $(d), $(TESTS_$(d))), \
	$(call compile_test,$(addsuffix .c, $(TEST)),$(TEST))))

# Now include any children we may have
MAKE_INCLUDES := $(wildcard $(d)*/Makefile)
$(eval $(foreach INC, $(MAKE_INCLUDES), $(call do_include,$(INC))))

# Finally, pop off the directory stack
d  := $(dirstack_$(sp))
sp := $(basename $(sp))
