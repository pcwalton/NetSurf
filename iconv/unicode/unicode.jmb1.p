Index: c/enc_utf8
===================================================================
RCS file: /home/rool/cvsroot/castle/RiscOS/Sources/Lib/Unicode/c/enc_utf8,v
retrieving revision 1.9
diff -u -r1.9 enc_utf8
--- c/enc_utf8	10 Jun 2002 15:08:35 -0000	1.9
+++ c/enc_utf8	19 Nov 2008 18:55:18 -0000
@@ -96,7 +96,7 @@
         }
         else
         {
-            if (c <= 0x80)
+            if (c < 0x80)
                 u = c;
             else if (c < 0xC0 || c >= 0xFE)
                 u = 0xFFFD;
Index: c/encoding
===================================================================
RCS file: /home/rool/cvsroot/castle/RiscOS/Sources/Lib/Unicode/c/encoding,v
retrieving revision 1.39
diff -u -r1.39 encoding
--- c/encoding	26 Aug 2005 15:02:17 -0000	1.39
+++ c/encoding	19 Nov 2008 18:55:19 -0000
@@ -67,20 +67,20 @@
 static EncList enclist[] =
 {
  {   csASCII /* 3 */, 1, "/US-ASCII/", lang_ENGLISH, &enc_ascii, NULL, NULL },
- {   csISOLatin1 /* 4 */, 1, "/ISO-8859-1/ISO-IR-100/", lang_ENGLISH, &enc_iso8859, "\x1B\x2D\x41\x1B\x2E\x42\x1B\x2F\x50", NULL }, /* Select G1 Latin-1, G2 Latin-2, G3 supplement */
- {   csISOLatin2 /* 5 */, 1, "/ISO-8859-2/ISO-IR-101/", lang_ENGLISH, &enc_iso8859, "\x1B\x2D\x42\x1B\x2E\x41\x1B\x2F\x50", NULL }, /* Select G1 Latin-2, G2 Latin-1, G3 supplement */
+ {   csISOLatin1 /* 4 */, 1, "/ISO-8859-1/ISO-IR-100/", lang_ENGLISH, &enc_iso8859, "\x1B\x2D\x41"/*\x1B\x2E\x42\x1B\x2F\x50"*/, NULL }, /* Select G1 Latin-1, G2 Latin-2, G3 supplement */
+ {   csISOLatin2 /* 5 */, 1, "/ISO-8859-2/ISO-IR-101/", lang_ENGLISH, &enc_iso8859, "\x1B\x2D\x42"/*\x1B\x2E\x41\x1B\x2F\x50"*/, NULL }, /* Select G1 Latin-2, G2 Latin-1, G3 supplement */
  {   csISOLatin3 /* 6 */, 1, "/ISO-8859-3/ISO-IR-109/", lang_ENGLISH, &enc_iso8859, "\x1B\x2D\x43", NULL },	                /* Select Latin-3 right half */
  {   csISOLatin4 /* 7 */, 1, "/ISO-8859-4/ISO-IR-110/", lang_ENGLISH, &enc_iso8859, "\x1B\x2D\x44", NULL },	                /* Select Latin-4 right half */
  {   csISOLatinCyrillic /* 8 */, 1, "/ISO-8859-5/ISO-IR-144/", lang_RUSSIAN, &enc_iso8859, "\x1B\x2D\x4C", NULL },		/* Select Cyrillic right half */
  {   csISOLatinGreek /* 10 */, 1, "/ISO-8859-7/ISO-IR-126/", lang_GREEK, &enc_iso8859, "\x1B\x2D\x46", NULL },		/* Select Greek right half */
  {   csISOLatinHebrew /* 11 */, 1, "/ISO-8859-8/ISO-IR-198/", lang_HEBREW, &enc_iso8859, "\x1B\x2D\x5E", NULL },		/* Select Hebrew right half */
- {   csISOLatin5 /* 12 */, 1, "/ISO-8859-9/ISO-IR-148/", lang_TURKISH, &enc_iso8859, "\x1B\x2D\x4D\x1B\x2E\x42\x1B\x2F\x50", NULL },	/* Select G1 Latin-5, G2 Latin-2, G3 supplement */
- {   csISOLatin6 /* 13 */, 1, "/ISO-8859-10/ISO-IR-157/", lang_ENGLISH, &enc_iso8859, "\x1B\x2D\x56\x1B\x2E\x58", NULL },	/* Select Latin-6 right half, and Sami supplement as G2 */
+ {   csISOLatin5 /* 12 */, 1, "/ISO-8859-9/ISO-IR-148/", lang_TURKISH, &enc_iso8859, "\x1B\x2D\x4D"/*\x1B\x2E\x42\x1B\x2F\x50"*/, NULL },	/* Select G1 Latin-5, G2 Latin-2, G3 supplement */
+ {   csISOLatin6 /* 13 */, 1, "/ISO-8859-10/ISO-IR-157/", lang_ENGLISH, &enc_iso8859, "\x1B\x2D\x56"/*\x1B\x2E\x58"*/, NULL },	/* Select Latin-6 right half, and Sami supplement as G2 */
  {   csISOLatinThai, 1, "/ISO-8859-11/ISO-IR-166/", lang_THAI, &enc_iso8859, "\x1B\x2D\x54", NULL },                          /* Select Thai right half */
  {   csISOLatin7, 1, "/ISO-8859-13/ISO-IR-179/", lang_ENGLISH, &enc_iso8859, "\x1B\x2D\x59", NULL },		                /* Select Baltic Rim right half */
  {   csISOLatin8, 1, "/ISO-8859-14/ISO-IR-199/", lang_IRISH, &enc_iso8859, "\x1B\x2D\x5F", NULL },	                        /* Select Celtic right half */
- {   csISOLatin9, 1, "/ISO-8859-15/ISO-IR-203/", lang_ENGLISH, &enc_iso8859, "\x1B\x2D\x62\x1B\x2E\x42\x1B\x2F\x50", NULL },  /* Select G1 Latin-9, G2 Latin-2, G3 supplement */
- {   csISOLatin10, 1, "/ISO-8859-16/ISO-IR-226/", lang_ENGLISH, &enc_iso8859, "\x1B\x2D\x66\x1B\x2E\x41\x1B\x2F\x50", NULL },  /* Select G1 Latin-10, G2 Latin-1, G3 supplement */
+ {   csISOLatin9, 1, "/ISO-8859-15/ISO-IR-203/", lang_ENGLISH, &enc_iso8859, "\x1B\x2D\x62"/*\x1B\x2E\x42\x1B\x2F\x50"*/, NULL },  /* Select G1 Latin-9, G2 Latin-2, G3 supplement */
+ {   csISOLatin10, 1, "/ISO-8859-16/ISO-IR-226/", lang_ENGLISH, &enc_iso8859, "\x1B\x2D\x66"/*\x1B\x2E\x41\x1B\x2F\x50"*/, NULL },  /* Select G1 Latin-10, G2 Latin-1, G3 supplement */
  {   csISO6937, 2, "/ISO-IR-156/", lang_ENGLISH, &enc_iso6937, "\x1B\x2D\x52", NULL },                         /* Select ISO6937 right half */
  {   csISO6937DVB, 2, "/X-ISO-6937-DVB/X-DVB/", lang_ENGLISH, &enc_iso6937, NULL, NULL },
  {   csShiftJIS /* 17 */, 2, "/SHIFT_JIS/X-SJIS/", lang_JAPANESE, &enc_shiftjis, NULL, NULL },
Index: c/iso2022
===================================================================
RCS file: /home/rool/cvsroot/castle/RiscOS/Sources/Lib/Unicode/c/iso2022,v
retrieving revision 1.19
diff -u -r1.19 iso2022
--- c/iso2022	25 Aug 2005 11:57:08 -0000	1.19
+++ c/iso2022	19 Nov 2008 18:55:19 -0000
@@ -32,6 +32,7 @@
 #include <stdio.h>
 #include <string.h>
 
+#include "charsets.h"
 #include "encpriv.h"
 #include "iso2022.h"
 
@@ -66,8 +67,24 @@
     unsigned char tempset;
     ISO2022_Set *oldset;
 
+    /* Whether escape sequences are disabled
+     * 
+     * Value: Meaning:
+     *   0    All escape sequences enabled 
+     *   1    Only SS2/3 escape sequences enabled
+     *   2    All escape sequences disabled
+     */
     unsigned char esc_disabled;
 
+    /* Whether C1 control characters are permitted 
+     *
+     * Value: Meaning:
+     *   0    No C1 control characters permitted
+     *   1    Only 0x8E/0x8F permitted 
+     *   2    All C1 control characters permitted
+     */
+    unsigned char c1_permitted;
+
     /* Pending escape commands */
     unsigned char esc_pending;
     unsigned char esc_multi;
@@ -136,7 +153,7 @@
     if (c == 0x00 || c == 0x5F)
     {
         *sync = 0;
-        return c + 0x20;
+        return invoker == _GL ? c + 0x20 : 0xFFFD;
     }
 
     if (!*sync)
@@ -150,8 +167,6 @@
         *sync = 0;
         return u;
     }
-
-    NOT_USED(invoker);
 }
 
 static UCS4 null_double_next_code(ISO2022_Set *s, int c, int invoker, unsigned char *sync)
@@ -379,6 +394,7 @@
     i->CR_s = C1;
     i->GR_s = G1;
 
+    i->c1_permitted = 2;
     i->esc_disabled = 0;
     i->esc_pending = i->esc_revision = 0;
     i->tempset = 0;
@@ -394,18 +410,39 @@
     iso2022_select_set(i, C0, 32, C0_ISO646);
     iso2022_select_set(i, C1, 32+1, C1_ISO6429);
 
-    /* ISO8859 and EUC variants of IOS2022 require preloading with
+    /* ISO8859 and EUC variants of ISO2022 require preloading with
        escape sequences to get the appropriate tables */
     if (e->list_entry->preload)
     {
+        char euc = 0;
 	unsigned int n = strlen(e->list_entry->preload);
 	if (n != e->read(e, NULL, (unsigned char *)e->list_entry->preload, n, NULL))
 	    return 0;
-        
-        /* if we've preloaded then we need to disable further escape
-         * sequences otherwise stray control sequences (eg 8E, 8F)
-         * will try and switch tables */
-        i->esc_disabled = 1;
+
+        if (e->list_entry->identifier == csEUCPkdFmtJapanese ||
+                /* e->list_entry->identifier == csKSC56011987 || */
+                e->list_entry->identifier == csEUCKR ||
+                e->list_entry->identifier == csGB2312)
+            euc = 1;       
+
+        /* If we've preloaded and we're not handling an EUC variant
+         * then we need to disable further escape sequences otherwise 
+         * stray control sequences (eg 8E, 8F) will try and switch tables.
+         *
+         * If we're handling an EUC variant which has loaded tables into 
+         * G2 and G3, then SS2/SS3 are permitted. */
+        if (euc && ((simple_set *)i->Set[G2])->table && 
+                ((simple_set *)i->Set[G3])->table)
+        {
+            i->esc_disabled = 1;
+            i->c1_permitted = 1;
+        }
+        else
+        {
+            i->esc_disabled = 2;
+            if (euc)
+                i->c1_permitted = 0;
+        }
     }
 
     if (for_encoding != encoding_READ)
@@ -600,11 +637,17 @@
 	UNIDBG(("iso2022: %02x\n", c));
 
 	/* check for illegal single shifts */
	if ((i->tempset == 1 && (c < 0x20 || c > 0x7F)) ||
 	    (i->tempset == 2 && (c < 0xA0)))
 	{
 	    u = 0xFFFD;
 	}
+        /* or illegal continuation bytes */
+        else if ((i->sync[_GL] && (c < 0x20 || c > 0x7F)) ||
+                 (i->sync[_GR] && (c < 0xA0)))
+        {
+            u = 0xFFFD;
+        }
 	else if (i->esc_pending)
 	{
             u = iso2022_esc_cont(i, c);
@@ -622,7 +665,11 @@
         else if (c < 0xA0)
         {
             i->sync[_GL] = i->sync[_GR] = 0;
-            u = i->CR->next_code(i->CR, c - 0x80, _CR, NULL);
+            if (i->c1_permitted == 2 || 
+                    (i->c1_permitted == 1 && (c == 0x8E || c == 0x8F)))
+                u = i->CR->next_code(i->CR, c - 0x80, _CR, NULL);
+            else
+                u = 0xFFFD;
         }
         else
         {
@@ -638,9 +685,9 @@
                        break;
             case 0x0E: if (!i->esc_disabled) { iso2022_ls(i, G1); continue; }
                        break;
-            case 0x8E: if (!i->esc_disabled) { iso2022_ss(i, G2); continue; }
+            case 0x8E: if (i->esc_disabled < 2) { iso2022_ss(i, G2); continue; }
                        break;
-            case 0x8F: if (!i->esc_disabled) { iso2022_ss(i, G3); continue; }
+            case 0x8F: if (i->esc_disabled < 2) { iso2022_ss(i, G3); continue; }
                        break;
         }
 
@@ -877,6 +927,9 @@
 
         /* UNIDBG(("scan_table: set %d table %p\n", set, setptr->table)); */
 
+        if (setptr->table == NULL)
+            continue;
+
 	if ((i = encoding_lookup_in_table(u, setptr->table)) != -1)
 	{
 	    *index = i;
@@ -907,8 +960,10 @@
 retry:
 
     /* control chars */
-    if (u < 0x0021)
+    if (u < 0x0021 || u == 0x007F)
 	buf[out++] = u;
+    else if ((enc->c1_permitted == 2 && 0x0080 <= u && u <= 0x009F))
+        buf[out++] = u;
 
     /* main chars */
     else if (iso2022_scan_sets(enc, u, &index, &set, &n_entries))
Index: c/iso6937
===================================================================
RCS file: /home/rool/cvsroot/castle/RiscOS/Sources/Lib/Unicode/c/iso6937,v
retrieving revision 1.1
diff -u -r1.1 iso6937
--- c/iso6937	25 Aug 2005 11:57:08 -0000	1.1
+++ c/iso6937	19 Nov 2008 18:55:20 -0000
@@ -354,11 +354,13 @@
 
 static int iso6937_find_accent_pair(UCS4 u)
 {
-    for (int a = 1; a <= 15; a++)
+    int a, i;
+
+    for (a = 1; a <= 15; a++)
     {
         if (iso6937_combination_table[a].combination)
         {
-            for (int i = 0; i < iso6937_combination_table[a].ncombinations; i++)
+            for (i = 0; i < iso6937_combination_table[a].ncombinations; i++)
             {
                 if (iso6937_combination_table[a].combination[i].u == u)
                 {
Index: c/johab
===================================================================
RCS file: /home/rool/cvsroot/castle/RiscOS/Sources/Lib/Unicode/c/johab,v
retrieving revision 1.6
diff -u -r1.6 johab
--- c/johab	10 Jun 2002 15:08:35 -0000	1.6
+++ c/johab	19 Nov 2008 18:55:20 -0000
@@ -116,10 +116,10 @@
             /* Hangul is --X */
             static const unsigned char final_only[28] =
             {
-                   0, 0,    0, 0x33,    0, 0x35, 0x36,    0,
-                   0, 0, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
-                0x40, 0,    0,    0, 0x44,    0,    0,    0,
-                   0, 0,    0,    0
+                   0,    0,    0, 0x33,    0, 0x35, 0x36,
+                   0,    0, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E,
+                0x3F, 0x40,    0,    0, 0x44,    0,    0,
+                   0,    0,    0,    0,    0,    0,    0
             };
 
             u = 0x3100 + final_only[final];
@@ -250,8 +250,10 @@
         }
         else
         {
-            if (c < 0x80)
+            if (c < 0x80 && c != 0x5C) /* Standard ASCII... */
                 u = c;
+            else if (c == 0x5C) /* ...except 0x5C, which maps to Won */
+                u = 0x20A9;
             else if ((c >= 0x84 && c <= 0xD3) ||
                      (c >= 0xD8 && c <= 0xDE) ||
                      (c >= 0xE0 && c <= 0xF9))
@@ -333,13 +335,15 @@
 static int johab_write(EncodingPriv *e, UCS4 u, unsigned char **johab, int *bufsize)
 {
     Johab_Encoding *je = (Johab_Encoding *) e;
-    int c = '?';
+    int c = 0xFFFD;
 
     if (u == NULL_UCS4)
 	return 0;
 
-    if (u <= 0x7F) /* Basic Latin */
+    if (u <= 0x7F && u != 0x5C) /* Basic Latin */
         c = u;
+    else if (u == 0x20A9) /* Won Sign, mapped to 0x5C */
+        c = 0x5C;
     else if (u >= 0xAC00 && u <= 0xD7A3) /* Hangul syllables */
         c = ucs_hangul_to_johab(u);
     else if (u >= 0x3131 && u <= 0x3163) /* Modern Jamo */
@@ -373,6 +377,11 @@
         }
     }
 
+    if (c == 0xFFFD && e->for_encoding == encoding_WRITE_STRICT)
+        return -1;
+    else if (c == 0xFFFD)
+        c = '?';
+
     if ((*bufsize -= (c > 0xFF ? 2 : 1)) < 0 || !johab)
 	return 0;
 
Index: c/shiftjis
===================================================================
RCS file: /home/rool/cvsroot/castle/RiscOS/Sources/Lib/Unicode/c/shiftjis,v
retrieving revision 1.13
diff -u -r1.13 shiftjis
--- c/shiftjis	10 Jun 2002 15:08:35 -0000	1.13
+++ c/shiftjis	19 Nov 2008 18:55:20 -0000
@@ -173,7 +173,7 @@
         else
         {
             if (c < 0x80)
-                u = c == 0x5C ? 0x00A5 : c; /* CP932 is as Basic Latin, except for yen */
+                u = c == 0x5C ? 0x00A5 : (c == 0x7E ? 0x203E : c); /* CP932 is as Basic Latin, except for yen and overbar */
             else if (c == 0x80)
                 u = 0x005C; /* Backslash - a Mac extension */
             else if (c < 0xA0)
@@ -217,7 +217,7 @@
 {
     int i;
 
-    if (u >= 0x21 && u <= 0x7E && u != 0x5C)  /* lower set is ASCII, except... */
+    if (u >= 0x21 && u < 0x7E && u != 0x5C)  /* lower set is ASCII, except... */
     {
         *table_no = 0;
         *index = u - 0x21;
@@ -231,6 +231,13 @@
         return 1;
     }
 
+    if (u == 0x203E) /* slot 7E is overbar */
+    {
+        *table_no = 0;
+        *index = 0x7E - 0x21;
+        return 1;
+    }
+
     if ((i = encoding_lookup_in_table(u, sj->katakana)) != -1)
     {
 	*table_no = 1;
Index: c/textconv
===================================================================
RCS file: /home/rool/cvsroot/castle/RiscOS/Sources/Lib/Unicode/c/textconv,v
retrieving revision 1.4
diff -u -r1.4 textconv
--- c/textconv	25 Aug 2005 11:57:08 -0000	1.4
+++ c/textconv	19 Nov 2008 18:55:20 -0000
@@ -67,8 +67,8 @@
 
 static int src_enc = csCurrent;
 static int dst_enc = csCurrent;
-static FILE *in = stdin;
-static FILE *out = stdout;
+static FILE *in;
+static FILE *out;
 static Encoding *read, *write;
 static char inbuf[256], outbuf[256];
 static unsigned int src_flags, dst_flags;
@@ -184,6 +184,15 @@
                 return 1;
             }
         }
+        else
+        {
+            out = stdout;
+        }
+    }
+    else
+    {
+        in = stdin;
+        out = stdout;
     }
 
     if (src_enc == dst_enc)
Index: c/unix
===================================================================
RCS file: /home/rool/cvsroot/castle/RiscOS/Sources/Lib/Unicode/c/unix,v
retrieving revision 1.3
diff -u -r1.3 unix
--- c/unix	5 Mar 2004 18:16:24 -0000	1.3
+++ c/unix	19 Nov 2008 18:55:20 -0000
@@ -33,6 +33,8 @@
 #include "layers_dbg.h"
 #endif
 
+#include <dirent.h>
+
 #include <string.h>
 #include <stdio.h>
 
@@ -40,9 +42,12 @@
 
 int encoding__load_map_file(const char *leaf, UCS2 **ptable, int *pn_entries, int *palloc)
 {
+    DIR *dir;
     FILE *fh;
     int flen;
     char fname[1024];
+    char *slash;
+    struct dirent *dp;
 
     void *table;
     int n_entries;
@@ -59,6 +64,31 @@
     strncat(fname, leaf, sizeof(fname));
     fname[sizeof(fname)-1] = 0;
 
+    /* We get to search the directory, because the leafname may be a prefix */
+    slash = fname + strlen(fname);
+    while (slash > fname && *slash != '/')
+        slash--;
+    if (slash == fname)
+        return 0;
+
+    *slash = '\0';
+    slash++;
+
+    dir = opendir(fname);
+    if (!dir)
+        return 0;
+
+    while ((dp = readdir(dir)) != NULL) {
+        if (strncmp(dp->d_name, slash, strlen(slash)) == 0) {
+            *(slash - 1) = '/';
+            *slash = '\0';
+            strncat(fname, dp->d_name, sizeof(fname));
+            break;
+        }
+    }
+
+    closedir(dir);
+
     fh = fopen(fname, "rb");
     if (!fh)
 	return 0;
Index: ccsolaris/Makefile
===================================================================
RCS file: /home/rool/cvsroot/castle/RiscOS/Sources/Lib/Unicode/ccsolaris/Makefile,v
retrieving revision 1.2
diff -u -r1.2 Makefile
--- ccsolaris/Makefile	25 Aug 2005 11:57:08 -0000	1.2
+++ ccsolaris/Makefile	19 Nov 2008 18:55:21 -0000
@@ -17,9 +17,25 @@
 # 
 # Project:   Unicode
 
-CC=gcc
+ifeq ($(findstring riscos,$(TARGET)),riscos)
+  GCCSDK_INSTALL_CROSSBIN ?= /home/riscos/cross/bin
 
-CCflags=-funsigned-char
+  CC = $(wildcard $(GCCSDK_INSTALL_CROSSBIN)/*gcc)
+
+  ifeq ($(findstring module,$(TARGET)),module)
+    PlatCCflags = -mmodule
+  endif
+
+  PlatObjs = riscos.o
+else
+  CC = gcc
+
+  PlatObjs = unix.o
+endif
+
+HOST_CC = gcc
+
+CCflags = -funsigned-char -g -O0 $(PlatCCflags)
 
 .c.o:;	$(CC) -c -DDEBUG=0 $(CCflags) -o $@ $<
 
@@ -43,7 +59,8 @@
 	enc_system.o \
 	acorn.o \
 	combine.o \
-	unix.o
+	debug.o \
+	$(PlatObjs)
 
 all:	ucodelib.a textconv
 
@@ -51,7 +68,10 @@
 	${AR} r $@ $(Objects)
 
 textconv: textconv.o ucodelib.a
-	${CC} -o $@ textconv.o ucodelib.a
+	${CC} $(CCflags) -o $@ textconv.o ucodelib.a
+
+mkunictype: mkunictype.c
+	${HOST_CC} -o $@ $<
 
 clean:
 	@-rm mkunictype textconv
