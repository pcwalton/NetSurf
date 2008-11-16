/* stateless 8bit encoding support => no support for CP1255, 1258 or TCVN
 * functions in this file have an identical API to the encoding functions
 * in UnicodeLib. see unicode/encoding.h for documentation. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"

#ifndef __riscos__
#define DIR_SEP "/"
#else
#define DIR_SEP "."
#endif

struct table_entry {
	const char *canon;
	const char *filename;
};

/* Table should be ordered by enc_num */
static const struct table_entry mapping_table[] = {
	{ "US-ASCII", 0 },
	{ "HP-ROMAN8", "HPR8" },
	{ "MACINTOSH", "Apple" DIR_SEP "Roman"},
	{ "IBM437", "Microsoft" DIR_SEP "CP437" },
	{ "IBM775", "Microsoft" DIR_SEP "CP775" },
	{ "IBM850", "Microsoft" DIR_SEP "CP850" },
	{ "IBM852", "Microsoft" DIR_SEP "CP852" },
	{ "IBM855", "Microsoft" DIR_SEP "CP855" },
	{ "IBM857", "Microsoft" DIR_SEP "CP857" },
	{ "IBM860", "Microsoft" DIR_SEP "CP860" },
	{ "IBM861", "Microsoft" DIR_SEP "CP861" },
	{ "IBM862", "Microsoft" DIR_SEP "CP862" },
	{ "IBM863", "Microsoft" DIR_SEP "CP863" },
	{ "IBM864", "Microsoft" DIR_SEP "CP864" },
	{ "IBM865", "Microsoft" DIR_SEP "CP865" },
	{ "IBM866", "Microsoft" DIR_SEP "CP866" },
	{ "IBM869", "Microsoft" DIR_SEP "CP869" },
	{ "KOI8-R", "KOI8-R" },
	{ "KOI8-U", "KOI8-U" },
	{ "IBM00858", "Microsoft" DIR_SEP "CP858" },
	{ "WINDOWS-1250", "Microsoft" DIR_SEP "CP1250" },
	{ "WINDOWS-1251", "Microsoft" DIR_SEP "CP1251" },
	{ "WINDOWS-1252", "Microsoft" DIR_SEP "CP1252" },
	{ "WINDOWS-1253", "Microsoft" DIR_SEP "CP1253" },
	{ "WINDOWS-1254", "Microsoft" DIR_SEP "CP1254" },
	{ "WINDOWS-1256", "Microsoft" DIR_SEP "CP1256" },
	{ "WINDOWS-1257", "Microsoft" DIR_SEP "CP1257" },
	{ "CP737", "Microsoft" DIR_SEP "CP737" },
	{ "CP853", "Microsoft" DIR_SEP "CP853" },
	{ "CP856", "Microsoft" DIR_SEP "CP856" },
	{ "CP874", "Microsoft" DIR_SEP "CP874" },
	{ "CP922", "Microsoft" DIR_SEP "CP922" },
	{ "CP1046", "Microsoft" DIR_SEP "CP1046" },
	{ "CP1124", "Microsoft" DIR_SEP "CP1124" },
	{ "CP1125", "Microsoft" DIR_SEP "CP1125" },
	{ "CP1129", "Microsoft" DIR_SEP "CP1129" },
	{ "CP1133", "Microsoft" DIR_SEP "CP1133" },
	{ "CP1161", "Microsoft" DIR_SEP "CP1161" },
	{ "CP1162", "Microsoft" DIR_SEP "CP1162" },
	{ "CP1163", "Microsoft" DIR_SEP "CP1163" },
	{ "GEORGIAN-ACADEMY", "GeorgA" },
	{ "GEORGIAN-PS", "GeorgPS" },
	{ "KOI8-RU", "KOI8-RU" },
	{ "KOI8-T", "KOI8-T" },
	{ "MACARABIC", "Apple" DIR_SEP "Arabic" },
	{ "MACCROATIAN", "Apple" DIR_SEP "Croatian" },
	{ "MACGREEK", "Apple" DIR_SEP "Greek" },
	{ "MACHEBREW", "Apple" DIR_SEP "Hebrew" },
	{ "MACICELAND", "Apple" DIR_SEP "Iceland" },
	{ "MACROMANIA", "Apple" DIR_SEP" Romania" },
	{ "MACTHAI", "Apple" DIR_SEP "Thai" },
	{ "MACTURKISH", "Apple" DIR_SEP "Turkish" },
	{ "MULELAO-1", "Mulelao" },
	{ "MACCYRILLIC", "Apple" DIR_SEP "Cyrillic" },
	{ "MACUKRAINE", "Apple" DIR_SEP "Ukrainian" },
	{ "MACCENTRALEUROPE", "Apple" DIR_SEP "CentEuro" },
};

#define TABLE_SIZE (sizeof(mapping_table) / sizeof(mapping_table[0]))

static const char *get_table_path(const char *table);

/**
 * Look up an encoding number, based on its name
 *
 * \param name  The encoding name
 * \return The encoding number, or 0 if not found
 */
int iconv_eightbit_number_from_name(const char *name)
{
	struct canon *c;
	int i;

	if (!name)
		return 0;

	c = alias_canonicalise(name);
	if (!c)
		return 0;

	LOG(("searching for: %s", name));

	for (i = 0; i != TABLE_SIZE; i++) {
		if (strcasecmp(mapping_table[i].canon, c->name) == 0) {
			LOG(("found: %d", c->mib_enum | (1<<30)));
			return c->mib_enum | (1<<30);
		}
	}

	return 0;
}

/**
 * Read an 8bit encoded string
 *
 * \param e  The encoding context
 * \param callback  Callback function to handle generated UCS characters
 * \param s  The input string
 * \param n  The length (in bytes) of the input
 * \param handle  Callback private data pointer
 * \return The number of characters processed
 */
unsigned iconv_eightbit_read(struct encoding_context *e,
		int (*callback)(void *handle, UCS4 c), const char *s,
		unsigned int n, void *handle)
{
	UCS4 c;
	unsigned int pos;

	if (!e || !callback || !s)
		return 0;

	for (pos = 0; pos != n; pos++) {

		c = ((unsigned char *) s)[pos];

		LOG(("read: %d (%d)", c, pos));

		if (c < 0x80) {
			/* ASCII */
			if (callback(handle, c)) {
				/* Used character, so update pos */
				pos++;
				break;
			}
		}
		else if (c < 0x100 && e->intab) {
			LOG(("maps to: %x", e->intab[c - 0x80]));
			/* Look up in mapping table */
			if (e->intab[c - 0x80] != 0xffff) {
				if (callback(handle, e->intab[c - 0x80])) {
					pos++;
					break;
				}
			}
			else {
				/* character not defined in this encoding */
				if (callback(handle, 0xfffd)) {
					pos++;
					break;
				}
			}
		} else {
			/* character not defined in this encoding */
			if (callback(handle, 0xfffd)) {
				pos++;
				break;
			}
		}
	}

	return pos;
}

/**
 * Write a UCS character in an 8bit encoding
 *
 * \param e  The encoding context
 * \param c  The UCS4 character
 * \param buf  Indirect pointer to output buffer
 * \param bufsize  Pointer to size of output buffer
 * \return 1 on success, 0 if bufsize is too small, -1 if unrepresentable.
 */
int iconv_eightbit_write(struct encoding_context *e, UCS4 c,
		char **buf, int *bufsize)
{
	int i;

	/* sanity check input */
	if (!e || !bufsize || !buf || !*buf)
		return 0;

	/* buffer full */
	if (--*bufsize < 0)
		return 0;

	if (c < 0x0080)
		/* ASCII */
		*(*buf)++ = (char)c;
	else {
		/* Perform reverse table lookup */
		for (i = 0; i != 0x80; i++) {
			if (e->outtab && e->outtab[i] == c) {
				*(*buf)++ = (char)(i+0x80);
				break;
			}
		}
		if (i == 0x80) {
			/* Nothing was written => fixup bufsize */
			++*bufsize;
			return -1;
		}
	}

	LOG(("written: %d", *(*buf-1)));

	return 1;
}

/**
 * Load an 8bit encoding
 *
 * \param enc_num  The encoding number to load
 * \return Pointer to lookup table for encoding, or NULL on error
 */
unsigned short *iconv_eightbit_new(int enc_num)
{
	const char *filename = NULL;
	const char *name;
	FILE *fp;
	unsigned int len;
	int i;
	unsigned short *ret;

	name = mibenum_to_name(enc_num);
	if (!name)
		return NULL;

	/* Lookup filename in table */
	for (i = 0; i != TABLE_SIZE; i++)
		if (strcasecmp(mapping_table[i].canon, name) == 0) {
			if (mapping_table[i].filename == 0)
				return NULL;

			filename = get_table_path(mapping_table[i].filename);

			break;
		}

	LOG(("opening: %s", filename));

	/* Open */
	fp = fopen(filename, "rb");
	if (!fp) {
		return NULL;
	}

	/* Get extent */
	fseek(fp, 0, SEEK_END);
	len = (unsigned int)ftell(fp);
	fseek(fp, 0, SEEK_SET);

	/* Unexpected length => give up */
	if (len != 256) {
		fclose(fp);
		return NULL;
	}

	/* Create buffer */
	ret = calloc(128, sizeof(short));
	if (!ret) {
		fclose(fp);
		return NULL;
	}

	fread(ret, 128, sizeof(short), fp);

	fclose(fp);

	return ret;
}

/**
 * Delete any 8bit encodings used by a context
 *
 * \param e  The encoding context
 */
void iconv_eightbit_delete(struct encoding_context *e)
{
	if (!e)
		return;

	if (e->intab)
		free(e->intab);
	if (e->outtab)
		free(e->outtab);
}

const char *get_table_path(const char *table)
{
	char *ucpath;
	int plen;
	static char path[4096];

	/* Get !Unicode resource path */
#ifdef __riscos__
	ucpath = getenv("Unicode$Path");
#else
	ucpath = getenv("UNICODE_DIR");
#endif

	if (ucpath == NULL)
		return NULL;

	strncpy(path, ucpath, sizeof(path));
	plen = strlen(ucpath);
#ifndef __riscos__
	if (path[plen - 1] != '/') {
		strncat(path, "/", sizeof(path) - plen - 1);
		plen += 1;
	}
#endif

	strncat(path, "Encodings" DIR_SEP, sizeof(path) - plen - 1);

	strncat(path, table, sizeof(path) - plen - 1);
	path[sizeof(path) - 1] = '\0';

	return path;
}

