#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unicode/charsets.h"
#include "unicode/encoding.h"

#include "internal.h"

struct table_entry {
	const char *alias;
	const char *encname;
};

/* This table contains special cases to allow us to use UnicodeLib sensibly. */
static const struct table_entry mapping_table[] = {
	{"/UTF-7/UNICODE-1-1-UTF-7/UNICODE-2-0-UTF-7/", "UTF-7" },
	{"/ISO-10646-UCS-4/UCS-4/UTF-32/", "ISO-10646-UCS-4" },
	{"/UTF-16/UCS-2/ISO-10646-UCS-2/UNICODE-1-1/UNICODE-2-0/", "UTF-16" },
	{"/ISO-2022/", "ISO-2022" },
};

#define TABLE_SIZE (sizeof(mapping_table) / sizeof(mapping_table[0]))

/**
 * Look up an encoding number, based on its name
 *
 * \param name The encoding name
 * \return The encoding number, or 0 if not found.
 */
int iconv_encoding_number_from_name(const char *name)
{
	unsigned int i;
	char buf[256];
	struct canon *c;

	if (!name)
		return 0;

	snprintf(buf, sizeof buf, "/%s/", name);

	/* convert to upper case */
	for (i = 0; i != strlen(buf); i++) {
		if (buf[i] >= 'a' && buf[i] <= 'z')
			buf[i] = buf[i] - 32;
	}

	for (i = 0; i != TABLE_SIZE; i++)
		if (strstr(mapping_table[i].alias, buf) != NULL)
			return encoding_number_from_name(mapping_table[i].encname);

	c = alias_canonicalise(name);
	if (!c)
		return 0;

	return encoding_number_from_name(c->name);
}

/**
 * Look up an encoding name, based on its MIB number
 *
 * \param number  The encoding MIB number
 * \return Pointer to encoding name, or NULL if not found
 */
const char *iconv_encoding_name_from_number(int number)
{
	const char *ret = NULL;
	/* This is a PITA - UnicodeLib doesn't have a call to do this,
	 * so implement it ourselves. */
	switch (number) {
		case csUnicode11UTF7:
			ret = mapping_table[0].alias;
			break;
		case csUCS4:
			ret = mapping_table[1].alias;
			break;
		case csUnicode11:
			ret = mapping_table[2].alias;
			break;
		case csVenturaMath:
			ret = mapping_table[3].alias;
			break;
		default:
			ret = mibenum_to_name(number);
			break;
	}

	return ret;
}
