# Tests
DIR_TEST_ITEMS := table-from:table-from.c table-to:table-to.c
DIR_TEST_ITEMS := $(DIR_TEST_ITEMS) genutf8:genutf8.c gengb18030z:gengb18030z.c

testGNU: $(DIR)
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< ASCII
#	/* General multi-byte encodings */
#	$(BUILDDIR)/test_GNU_genutf8 > $(CURDIR)/$<UTF-8.TXT && $(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< UTF-8
#	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< UCS-2BE
#	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< UCS-2LE
#	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< UCS-4BE
#	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< UCS-4LE
	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< UTF-16
	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< UTF-16BE
	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< UTF-16LE
	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< UTF-32
	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< UTF-32BE
	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< UTF-32LE
	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< UTF-7
#	/* 8-bit encodings */
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< ISO-8859-1
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< ISO-8859-2
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< ISO-8859-3
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< ISO-8859-4
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< ISO-8859-5
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< ISO-8859-6
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< ISO-8859-7
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< ISO-8859-8
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< ISO-8859-9
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< ISO-8859-10
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< ISO-8859-11
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< ISO-8859-13
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< ISO-8859-14
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< ISO-8859-15
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< ISO-8859-16
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< KOI8-R
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< KOI8-U
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< KOI8-RU
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< CP1250
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< CP1251
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< CP1252
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< CP1253
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< CP1254
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< CP1255
#	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< CP1255
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< CP1256
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< CP1257
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< CP1258
#	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< CP1258
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< CP850
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< CP862
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< CP866
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< MacRoman
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< MacCentralEurope
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< MacIceland
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< MacCroatian
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< MacRomania
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< MacCyrillic
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< MacUkraine
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< MacGreek
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< MacTurkish
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< MacHebrew
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< MacArabic
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< MacThai
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< HP-ROMAN8
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< NEXTSTEP
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< ARMSCII-8
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< Georgian-Academy
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< Georgian-PS
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< KOI8-T
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< PT154
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< RK1048
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< MuleLao-1
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< CP1133
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< TIS-620
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< CP874
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< VISCII
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< TCVN
#	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< TCVN
#	/* CJK character sets */
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< ISO646-JP
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< JIS_X0201
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< JIS_X0208 # redundant, see EUC-JP
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< JIS_X0212 # redundant, see EUC-JP
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< ISO646-CN
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< GB_2312-80 # redundant, see EUC-CN
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< ISO-IR-165
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< KSC_5601 # redundant, see EUC-KR
#	/* CJK encodings */
	-$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< EUC-JP
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< SHIFT_JIS
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< CP932
	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< ISO-2022-JP
	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< ISO-2022-JP-2
	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< ISO-2022-JP-1
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< EUC-CN
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< GBK
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< CP936
#	{ cat $(CURDIR)/$<GB18030-BMP.TXT ; $(CURDIR)/$<test_GNU_gengb18030z ; } | sort > $(CURDIR)/$<GB18030.TXT && $(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< GB18030
	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< ISO-2022-CN
	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< ISO-2022-CN-EXT
#	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< HZ
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< EUC-TW
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< BIG5
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< CP950
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< BIG5-HKSCS:1999
#	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< BIG5-HKSCS:1999
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< BIG5-HKSCS:2001
#	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< BIG5-HKSCS:2001
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< BIG5-HKSCS:2004
#	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< BIG5-HKSCS:2004
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< EUC-KR
#	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< CP949
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< JOHAB
	$(CURDIR)/$<check-stateful $(BUILDDIR) "$(EXEEXT)" $(CURDIR)/$< ISO-2022-KR
#	/* transliteration */
#	$(CURDIR)/$<check-translit $(CURDIR)/$< Quotes UTF-8 ISO-8859-1
#	$(CURDIR)/$<check-translit $(CURDIR)/$< Quotes UTF-8 ASCII
#	$(CURDIR)/$<check-translit $(CURDIR)/$< Translit1 ISO-8859-1 ASCII
#	$(CURDIR)/$<check-translitfailure $(CURDIR)/$< TranslitFail1 ISO-8859-1 ASCII
#	/* substitution */
#	$(CURDIR)/$<check-subst
#	/* system dependent and extra encodings */
#	/* AIX specific encodings */
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP856
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP922
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP943
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP1046
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP1124
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP1129
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP1161
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP1162
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP1163
#	/* OSF/1 specific encodings */
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< DEC-KANJI
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< DEC-HANYU
#	/* DOS specific encodings */
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP437
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP737
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP775
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP852
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP853
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP855
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP857
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP858
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP860
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP861
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP863
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP864
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP865
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP869
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< CP1125
#	/* Extra encodings */
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< EUC-JISX0213
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< SHIFT_JISX0213
#	$(CURDIR)/$<check-stateful $(CURDIR) $(CURDIR)/$< ISO-2022-JP-3
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< BIG5-2003
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< TDS565
#	$(CURDIR)/$<check-stateless $(CURDIR) $(CURDIR)/$< ATARIST
	$(CURDIR)/$<check-stateless $(BUILDDIR) $(CURDIR)/$< X-ACORN-LATIN1

TEST_TARGETS := $(TEST_TARGETS) testGNU

DISTCLEAN_ITEMS := $(DISTCLEAN_ITEMS) $(DIR)/UTF-8.TXT $(DIR)/GB18030.TXT

include build/makefiles/Makefile.subdir
