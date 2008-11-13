/* stateless 8bit encoding support => no support for CP1255, 1258 or TCVN
 * functions in this file have an identical API to the encoding functions
 * in UnicodeLib. see unicode/encoding.h for documentation. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"

struct table_entry {
	const char *canon;
	const char *filename;
};

/* Table should be ordered by enc_num */
static const struct table_entry mapping_table[] = {
	{ "US-ASCII", 0 },
	{ "HP-ROMAN8", "HPR8" },
	{ "MACINTOSH", "Apple.Roman"},
	{ "IBM437", "Microsoft.CP437" },
	{ "IBM775", "Microsoft.CP775" },
	{ "IBM850", "Microsoft.CP850" },
	{ "IBM852", "Microsoft.CP852" },
	{ "IBM855", "Microsoft.CP855" },
	{ "IBM857", "Microsoft.CP857" },
	{ "IBM860", "Microsoft.CP860" },
	{ "IBM861", "Microsoft.CP861" },
	{ "IBM862", "Microsoft.CP862" },
	{ "IBM863", "Microsoft.CP863" },
	{ "IBM864", "Microsoft.CP864" },
	{ "IBM865", "Microsoft.CP865" },
	{ "IBM866", "Microsoft.CP866" },
	{ "IBM869", "Microsoft.CP869" },
	{ "KOI8-R", "KOI8-R" },
	{ "KOI8-U", "KOI8-U" },
	{ "IBM00858", "Microsoft.CP858" },
	{ "WINDOWS-1250", "Microsoft.CP1250" },
	{ "WINDOWS-1251", "Microsoft.CP1251" },
	{ "WINDOWS-1252", "Microsoft.CP1252" },
	{ "WINDOWS-1253", "Microsoft.CP1253" },
	{ "WINDOWS-1254", "Microsoft.CP1254" },
	{ "WINDOWS-1256", "Microsoft.CP1256" },
	{ "WINDOWS-1257", "Microsoft.CP1257" },
	{ "CP737", "Microsoft.CP737" },
	{ "CP853", "Microsoft.CP853" },
	{ "CP856", "Microsoft.CP856" },
	{ "CP874", "Microsoft.CP874" },
	{ "CP922", "Microsoft.CP922" },
	{ "CP1046", "Microsoft.CP1046" },
	{ "CP1124", "Microsoft.CP1124" },
	{ "CP1125", "Microsoft.CP1125" },
	{ "CP1129", "Microsoft.CP1129" },
	{ "CP1133", "Microsoft.CP1133" },
	{ "CP1161", "Microsoft.CP1161" },
	{ "CP1162", "Microsoft.CP1162" },
	{ "CP1163", "Microsoft.CP1163" },
	{ "GEORGIAN-ACADEMY", "GeorgA" },
	{ "GEORGIAN-PS", "GeorgPS" },
	{ "KOI8-RU", "KOI8-RU" },
	{ "KOI8-T", "KOI8-T" },
	{ "MACARABIC", "Apple.Arabic" },
	{ "MACCROATIAN", "Apple.Croatian" },
	{ "MACGREEK", "Apple.Greek" },
	{ "MACHEBREW", "Apple.Hebrew" },
	{ "MACICELAND", "Apple.Iceland" },
	{ "MACROMANIA", "Apple.Romania" },
	{ "MACTHAI", "Apple.Thai" },
	{ "MACTURKISH", "Apple.Turkish" },
	{ "MULELAO-1", "Mulelao" },
	{ "MACCYRILLIC", "Apple.Cyrillic" },
	{ "MACUKRAINE", "Apple.Ukrainian" },
	{ "MACCENTRALEUROPE", "Apple.CentEuro" },
};

#define TABLE_SIZE (sizeof(mapping_table) / sizeof(mapping_table[0]))

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

		c = s[pos];

		LOG(("read: %d (%d)", c, pos));

		if (c < 0x80) {
			/* ASCII */
			if (callback(handle, c))
				break;
		}
		else if (c < 0x100 && e->intab) {
			LOG(("maps to: %x", e->intab[c - 0x80]));
			/* Look up in mapping table */
			if (e->intab[c - 0x80] != 0xffff) {
				if (callback(handle, e->intab[c - 0x80]))
					break;
			}
			else {
				/* character not defined in this encoding */
				return pos;
			}
		} else {
			/* character not defined in this encoding */
			break;
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
	char filename[64];
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

			snprintf(filename, sizeof filename,
				"Unicode:Encodings.%s",
				mapping_table[i].filename);

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
