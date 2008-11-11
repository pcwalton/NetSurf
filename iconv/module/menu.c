/* Encoding menu */

#include <ctype.h>
#include <inttypes.h>
#ifdef MTEST
#include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>

#include <unicode/charsets.h>
#include <unicode/encoding.h>

#include "module.h"

#define menu_HEADER_SIZE (28)
#define menu_ENTRY_SIZE (24)

typedef struct _menu {
	char head[menu_HEADER_SIZE]; /* We don't care about this */
	struct {
		int menu_flags;
		int *sub_menu;
		int icon_flags;
		struct {		/* Only handle indirected text */
			char *text;
			char *validation;
			int size;
		} indirected_text;
	} entries[1];
} wimp_menu;

struct menu_desc {
	const char *title;
	int n_entries;
	const char *entries[1];
};

#define menudesc(N)							\
	struct {							\
		const char *title;					\
		int n_entries;						\
		const char *entries[(N)];				\
	}

/* Menu descriptions.
 * A number of magic characters are permitted at the start of entry names:
 *
 * Character:		Meaning:
 *    ^      		Insert separator after this entry
 *    >      		Can open submenu, even if shaded
 *    {..}   		Has submenu, named ".."
 *    $      		This entry is shaded
 *
 * Magic characters are examined in the order above.
 * The submenu name is the title of the submenu.
 * The first alphanumeric character is taken as the start of the entry name.
 */
static const char *val = ""; /* Validation string */

#define N_LATIN (30)
static const menudesc(N_LATIN) latin_menu = {
  "Latin", N_LATIN,
  {
    "Western (ISO 8859-1: Latin-1)",
    "Eastern (ISO 8859-2: Latin-2)",
    "Southern (ISO 8859-3: Latin-3)",
    "Nordic (ISO 8859-4: Latin-4)",
    "Turkish (ISO 8859-9: Latin-5)",
    "Nordic (ISO 8859-10: Latin-6)",
    "Baltic rim (ISO 8859-13: Latin-7)",
    "Celtic (ISO 8859-14: Latin-8)",
    "^Western (ISO 8859-15: Latin-9)",
    "Welsh (ISO-IR 182)",
    "^Sami (ISO-IR 197)",
    "Microsoft Latin-1 (CP1252)",
    "Microsoft Latin-2 (CP1250)",
    "Microsoft Baltic (CP1257)",
    "^Microsoft Turkish (CP1254)",
    "Apple Macintosh Roman",
    "Apple Macintosh Croatian",
    "Apple Macintosh Icelandic",
    "Apple Macintosh Romanian",
    "Apple Macintosh Turkish",
    "^Apple Macintosh Central European",
    "^Acorn Latin-1",
    "DOS Latin-1 (CP850)",
    "DOS Latin-2 (CP852)",
    "DOS Baltic rim (CP775)",
    "DOS Turkish (CP857)",
    "DOS Portuguese (CP860)",
    "DOS Icelandic (CP861)",
    "DOS CanadaF (CP863)",
    "DOS Nordic (CP865)",
  }
};

#define N_ARABIC (4)
static const menudesc(N_ARABIC) arabic_menu = {
  "Arabic", N_ARABIC,
  {
    "$ISO 8859-6",
    "Microsoft Arabic (CP1256)",
    "Apple Macintosh Arabic",
    "DOS Arabic (CP864)",
  }
};

#define N_CYRILLIC (10)
static const menudesc(N_CYRILLIC) cyrillic_menu = {
  "Cyrillic", N_CYRILLIC,
  {
    "ISO 8859-5",
    "KOI8-R",
    "KOI8-RU",
    "KOI8-T",
    "^KOI8-U",
    "^Microsoft Cyrillic (CP1251)",
    "Apple Macintosh Cyrillic",
    "^Apple Macintosh Ukrainian",
    "DOS Cyrillic (CP855)",
    "DOS Cyrillic Russian (CP866)",
  }
};

#define N_GREEK (5)
static const menudesc(N_GREEK) greek_menu = {
  "Greek", N_GREEK,
  {
    "ISO 8859-7",
    "Microsoft Greek (CP1253)",
    "Apple Macintosh Greek",
    "DOS Greek (CP737)",
    "DOS Greek2 (CP869)",
  }
};

#define N_HEBREW (4)
static const menudesc(N_HEBREW) hebrew_menu = {
  "Hebrew", N_HEBREW,
  {
    "ISO 8859-8",
    "$Microsoft Hebrew (CP1255)",
    "Apple Macintosh Hebrew",
    "DOS Hebrew (CP862)",
  }
};

#define N_CHINESE (3)
static const menudesc(N_CHINESE) chinese_menu = {
  "Chinese", N_CHINESE,
  {
    "ISO 2022-CN",
    "^GB 2312 (EUC-CN)",
    "Big Five",
  }
};

#define N_JAPANESE (3)
static const menudesc(N_JAPANESE) japanese_menu = {
  "Japanese", N_JAPANESE,
  {
    "ISO 2022-JP",
    "EUC-JP",
    "Shift-JIS",
  }
};

#define N_KOREAN (3)
static const menudesc(N_KOREAN) korean_menu = {
  "Korean", N_KOREAN,
  {
    "ISO 2022-KR",
    "EUC-KR",
    "Johab",
  }
};

#define N_THAI (3)
static const menudesc(N_THAI) thai_menu = {
  "Thai", N_THAI,
  {
    "ISO 8859-11",
    "Apple Macintosh Thai",
    "DOS Thai (CP874)",
  }
};

#define N_VIETNAMESE (1)
static const menudesc(N_VIETNAMESE) vietnamese_menu = {
  "Vietnamese", N_VIETNAMESE,
  {
    "$Microsoft Vietnamese (CP1258)",
  }
};

#define N_UNIVERSAL (4)
static const menudesc(N_UNIVERSAL) universal_menu = {
  "Universal", N_UNIVERSAL,
  {
    "UTF-8 (ASCII-compatible)",
    "UCS-2 / UTF-16 (16-bit)",
    "^UCS-4 (31-bit)",
    "ISO-2022",
  }
};

#define N_ENC (11)
static const menudesc(N_ENC) enc_menu = {
  "Encodings", N_ENC,
  {
    "^{Latin}Latin",
    "{Arabic}Arabic",
    "{Cyrillic}Cyrillic",
    "{Greek}Greek",
    "^{Hebrew}Hebrew",
    "{Chinese}Chinese",
    "{Japanese}Japanese",
    "{Korean}Korean",
    "{Thai}Thai",
    "^>{Vietnamese}$Vietnamese",
    "{Universal}Universal",
  }
};

/* This struct is a lookup table between menu entries and charset numbers
 * It is ordered as per the menus. */
static const struct csmap {
	short latin[N_LATIN];
	short arabic[N_ARABIC];
	short greek[N_GREEK];
	short hebrew[N_HEBREW];
	short cyrillic[N_CYRILLIC];
	short chinese[N_CHINESE];
	short japanese[N_JAPANESE];
	short korean[N_KOREAN];
	short thai[N_THAI];
	short vietnamese[N_VIETNAMESE];
	short universal[N_UNIVERSAL];
} csmap = {
	{ csISOLatin1, csISOLatin2, csISOLatin3, csISOLatin4, csISOLatin5,
	  csISOLatin6, csISOLatin7, csISOLatin8, csISOLatin9, csWelsh,
	  csSami, csWindows1252, csWindows1250, csWindows1257, csWindows1254,
	  csMacintosh, 3019, 3022, 3023, 3025, csMacCentEuro, csAcornLatin1,
	  csPC850Multilingual, csPCp852, csPC775Baltic, csIBM857, csIBM860,
	  csIBM861, csIBM863, csIBM865 },
	{ csISOLatinArabic, csWindows1256, 3018, csIBM864 },
	{ csISOLatinGreek, csWindows1253, 3020, 3000, csIBM869 },
	{ csISOLatinHebrew, csWindows1255, 3021, csPC862LatinHebrew },
	{ csISOLatinCyrillic, csKOI8R, 3016, 3017, 2088, csWindows1251,
	  csMacCyrillic, csMacUkrainian, csIBM855, csIBM866 },
	{ csISO2022CN, csGB2312, csBig5 },
	{ csISO2022JP, csEUCPkdFmtJapanese, csShiftJIS },
	{ csISO2022KR, csEUCKR, csJohab },
	{ csISOLatinThai, 3024, 3004 },
	{ csWindows1258 },
	{ csUTF8, csUnicode11, csUCS4, csVenturaMath }
};

/* Sub menu lookup table - Must be sorted alphabetically */
static const struct sub_menu {
	char name[12];
	const struct menu_desc *desc;
	const short *lut;
} sub_menus[] = {
	{ "Arabic", (const struct menu_desc *) (void *) &arabic_menu,
							csmap.arabic },
	{ "Chinese", (const struct menu_desc *) (void *) &chinese_menu,
							csmap.chinese },
	{ "Cyrillic", (const struct menu_desc *) (void *) &cyrillic_menu,
							csmap.cyrillic },
	{ "Greek", (const struct menu_desc *) (void *) &greek_menu, 
							csmap.greek },
	{ "Hebrew", (const struct menu_desc *) (void *) &hebrew_menu,
							csmap.hebrew },
	{ "Japanese", (const struct menu_desc *) (void *) &japanese_menu,
							csmap.japanese },
	{ "Korean", (const struct menu_desc *) (void *) &korean_menu,
							csmap.korean },
	{ "Latin", (const struct menu_desc *) (void *) &latin_menu, 
							csmap.latin },
	{ "Thai", (const struct menu_desc *) (void *) &thai_menu, csmap.thai },
	{ "Universal", (const struct menu_desc *) (void *) &universal_menu,
							csmap.universal },
	{ "Vietnamese", (const struct menu_desc *) (void *) &vietnamese_menu,
							csmap.vietnamese },
};
#define SUB_MENU_COUNT (sizeof(sub_menus) / sizeof(sub_menus[0]))



#define MAX_SUBMENUS (16) /* Maximum number of submenus each menu can have */

#define MENU_COUNT_SIZE (0x00)
#define MENU_CREATE (0x01)
#define MENU_CLEAR_SELECTIONS (0x02)
/**
 * Perform an operation on a menu
 *
 * \param d  The description
 * \param buf  Location to write menu to
 * \param parent  Parent menu
 * \param which  Which parent entry this menu is linked from
 * \param flags  Flags word
 *               Bit:             Meaning:
 *                0               Create menu
 *                1               Clear existing selections (charset != 0)
 * \param charset  Charset identifier of selected charset
 * \param lut Selection lookup table
 * \param data Location to write indirected data to
 * \return Pointer to location after menu data
 */
static char *menu_op(const struct menu_desc *d, char *buf,
		wimp_menu *parent, size_t which, size_t flags,
		short charset, const short *lut, char **data)
{
	int e, top = 0;
	struct { int e; const char *name; } submenus[MAX_SUBMENUS];
	char *bp = buf;
	char *dp;

	if (data)
		dp = *data;

	if (!buf && (flags & MENU_CLEAR_SELECTIONS))
		return buf;

	if ((flags & MENU_CREATE)) {
		/* copy menu title */
		strncpy(bp, d->title, 12);
		bp += 12;

		/* colours */
		*bp++ = 7; *bp++ = 2; *bp++ = 7; *bp++ = 0;

		/* width, height, gap */
		*((int *)bp) = 200; bp += 4;
		*((int *)bp) = 44;  bp += 4;
		*((int *)bp) = 0;   bp += 4;

		memcpy(dp, val, strlen(val) + 1);
		dp += strlen(val) + 1;
	} else {
		bp += menu_HEADER_SIZE;
		dp += strlen(val) + 1;
	}

	/* now the entries */
	for (e = 0; e != d->n_entries; e++) {
		int menuf = 0, icon = (7 << 24) | 0x121;
		const char *pos = 0;

		/* parse description string */
		for (pos = d->entries[e]; !isalnum(*pos); pos++) {
			if (*pos == '^')
				menuf |= 0x2;
			else if (*pos == '>')
				menuf |= 0x10;
			else if (*pos == '{') {
				if (top < MAX_SUBMENUS) {
					submenus[top].e = e;
					submenus[top++].name = pos+1;
				}
				while (*pos != '}')
					pos++;
			}
			else if (*pos == '$')
				icon |= (1<<22);
		}

		if (e == d->n_entries - 1) {
			/* last item */
			menuf |= 0x80;
		}

		if (charset != 0 && lut && lut[e] == charset) {
			menuf |= 0x1;
			if (parent)
				parent->entries[which].menu_flags |= 0x1;
		}
		else
			menuf &= ~0x1;

		if (flags & MENU_CLEAR_SELECTIONS) {
			((wimp_menu *)buf)->entries[e].menu_flags = menuf;
		}

		if ((flags & MENU_CREATE)) {
			*((int *)bp) = menuf;             bp += 4;
			*((int *)bp) = -1;                bp += 4;
			*((int *)bp) = icon;              bp += 4;
			*((int *)bp) = (intptr_t)(dp);    bp += 4;
			*((int *)bp) = (intptr_t)(*data); bp += 4;
			*((int *)bp) = strlen(pos) + 1;   bp += 4;

			memcpy(dp, pos, strlen(pos) + 1);
			dp += strlen(pos) + 1;
		} else {
			bp += menu_ENTRY_SIZE;
			dp += strlen(pos) + 1;
		}
	}

	/* fixup parent's pointer to this menu */
	if (parent && (flags & MENU_CREATE))
		parent->entries[which].sub_menu = (int *)buf;

	/* and recurse */
	for (e = 0; e < top; e++) {
		struct sub_menu *s;
		size_t len = (strchr(submenus[e].name, '}') -
							submenus[e].name);
		char child[len + 1];

		strncpy(child, submenus[e].name, len);
		child[len] = '\0';

		s = bsearch(child, sub_menus, SUB_MENU_COUNT,
				sizeof(sub_menus[0]),
				(int (*)(const void *, const void *))strcmp);
		if (s)
			bp = menu_op(s->desc, bp, (wimp_menu *)buf,
						submenus[e].e, flags,
						charset, s->lut, &dp);
	}

	if (data)
		(*data) = dp;

	return bp;
}

/**
 * Iconv_CreateMenu SWI - Creates a menu structure of supported encodings
 *
 * \param flags  Flags word - all reserved
 * \param buf  Pointer to buffer in which to store menu data, or NULL to
 *             read required buffer size.
 * \param len  Length of buffer, in bytes
 * \param selected  Pointer to name of selected encoding, or NULL if none
 * \param data Pointer to buffer in which to store indirected data, or NULL
 *             to read required buffer size.
 * \param dlen Pointer to length of data buffer, in bytes
 * \return length of data written in buffer, or 0 if insufficient space
 */
size_t iconv_createmenu(size_t flags, char *buf, size_t len,
		const char *selected, char *data, size_t *dlen)
{
	size_t reqlen, datalen;
	char *bp = buf, *dp = NULL;
	int sel = 0;
	struct canon *c;

	UNUSED(flags);

	/* sanity check arguments */
	if ((!buf && data) || !dlen)
		return 0;

	/* get required size */
	reqlen = (size_t)menu_op((const struct menu_desc *) (void *) &enc_menu, 
			0, NULL, 0, MENU_COUNT_SIZE, 0, NULL, &dp);

	datalen = (size_t)dp;

	/* buffer length requested, so return it */
	if (!buf) {
		*dlen = datalen;
		return reqlen;
	}

	/* insufficient room in buffer */
	if (reqlen > len)
		return 0;

	/* Selected entry? */
	if (selected) {
		sel = iconv_eightbit_number_from_name(selected) & ~(1<<30);

		if (!sel) {
			c = alias_canonicalise(selected);
			if (c) {
				sel = encoding_number_from_name(c->name);
			}
		}
	}

#ifdef TEST
	printf("selected: '%s' : %d\n", selected, sel);
#endif

	dp = data;
	bp = menu_op((const struct menu_desc *) (void *) &enc_menu, buf,
			NULL, 0, MENU_CREATE, sel, NULL, &dp);

	(*dlen) = datalen;

	return reqlen;
}

/**
 * Iconv_DecodeMenu SWI - Decodes a selection in a menu generated by
 * Iconv_CreateMenu.
 *
 * \param flags  Bitfield of flags - all reserved
 * \param menu   Menu definition
 * \param selections  Menu selections
 * \param buf    Pointer to output buffer, or NULL to read required length
 * \param buflen  Length of output buffer
 * \return Required length of output buffer, or 0 if no selections
 */
size_t iconv_decodemenu(size_t flags, void *menu, int *selections,
		char *buf, size_t buflen)
{
	const char *text, *t;
	size_t len;
	struct sub_menu *s;

	UNUSED(flags);

	if (!menu || !selections)
		return 0;

	/* out of range */
	if (selections[0] == -1 || selections[0] >= enc_menu.n_entries)
		return 0;

	/* Grab sub menu name */
	t = strchr(enc_menu.entries[selections[0]], '{') + 1;
	len = (strchr(t, '}') - t);

	/* copy to temporary buffer */
	char child[len + 1];
	strncpy(child, t, len);
	child[len] = '\0';

	/* look for submenu */
	s = bsearch(child, sub_menus, SUB_MENU_COUNT, sizeof(sub_menus[0]),
				(int (*)(const void *, const void *))strcmp);
	if (!s)
		return 0;

	if (selections[1] == -1 || selections[1] >= s->desc->n_entries)
		return 0;

	/* lookup encoding name from number */
	text = mibenum_to_name(s->lut[selections[1]]);

	/* not found */
	if (!text)
		return 0;

#ifdef MTEST
	printf("%p : '%s'\n", text, text);
#endif

	if (buf && buflen < strlen(text) + 1)
		/* insufficient buffer space */
		return 0;


	if (buf) {
		strcpy(buf, text);
		buf[strlen(text)] = '\0';
	}

	menu_op((const struct menu_desc *) (void *) &enc_menu, menu, NULL, 0,
			MENU_CLEAR_SELECTIONS, s->lut[selections[1]],
			NULL, NULL);

	return strlen(text) + 1;
}


#ifdef MTEST
int main(void)
{
	int len, slen, dlen;
	char *buf, *dbuf, *selected;
	int selection[3] = { 0, 5, -1};

	if (!create_alias_data("Unicode:Files.Aliases"))
		return 1;


	len = iconv_createmenu(0, 0, 0, 0, 0, (size_t *)&dlen);

	buf = calloc(len, sizeof(char));
	if (!buf)
		return 1;

	dbuf = calloc(dlen, sizeof(char));
	if (!dbuf)
		return 1;

	printf("%p: %d\n", buf, iconv_createmenu(0, buf, len, "UTF-16",
					dbuf, (size_t *)&dlen));

	FILE *fp = fopen("$.dump", "w");
	fwrite(buf, len, sizeof(char), fp);
	fclose(fp);

	fp = fopen("$.dump1", "w");
	fwrite(dbuf, dlen, sizeof(char), fp);
	fclose(fp);

	slen = iconv_decodemenu(0, (wimp_menu*)buf, selection, 0, 0);

	selected = calloc(slen, sizeof(char));
	if (!selected)
		return 1;

	printf("%p: %d\n", selected, iconv_decodemenu(0, (wimp_menu*)buf,
						selection, selected, slen));

	printf("'%s'\n", selected);

	free_alias_data();

	return 0;
}
#endif
