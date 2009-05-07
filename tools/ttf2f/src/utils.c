#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

char *strndup(const char *s, size_t n)
{
	/* this assumes s is NUL terminated - if not,
	 * some silly value will be returned */
	size_t len = strlen(s);
	char *res;

	/* limit to n */
	if (len > n || n == 0)
		len = n;

	res = (char *) malloc(len + 1);
	if (res == NULL)
		return NULL;

	res[len] = '\0';
	return (char *) memcpy(res, s, len);
}

/**
 * Convert raw font units to units normalised with a design size of 1000
 *
 * \param raw  Raw value from file
 * \param ppem Design size of font
 * \return Converted value
 */
long convert_units(long raw, long ppem)
{
	if (ppem == 1000)
		return raw;

	return (raw * 1000) / ppem;
}
