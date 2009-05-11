#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

char *strndup(const char *s, size_t n)
{
	size_t len = 0;
	char *res;

	while (len < n && s[len] != '\0')
		len++;

	res = malloc(len + 1);
	if (res == NULL)
		return NULL;

	res[len] = '\0';
	return memcpy(res, s, len);
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
