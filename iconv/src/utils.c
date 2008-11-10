#include <ctype.h>

#include "internal.h"

/**
 * Case insensitive string comparison
 *
 * \param s1 Pointer to string
 * \param s2 Pointer to string
 * \return 0 if strings match, <> 0 if no match
 */
int strcasecmp(const char *s1, const char *s2)
{
	int i;

	if (!s1 || !s2)
		return 1; /* this is arbitrary */

	if (s1 == s2)
		return 0;

	while ((i = tolower(*s1)) && i == tolower(*s2))
		s1++, s2++;

	return ((unsigned char) tolower(*s1) - (unsigned char) tolower(*s2));
}

/**
 * Length-limited case insensitive string comparison
 *
 * \param s1 Pointer to string
 * \param s2 Pointer to string
 * \param len Length to compare
 * \return 0 if strings match, <> 0 if no match
 */
int strncasecmp(const char *s1, const char *s2, size_t len)
{
	int i;

	if (!s1 || !s2)
		return 1; /* this is arbitrary */

	if (len == 0)
		return 0;

	if (s1 == s2)
		return 0;

	while (len-- && (i = tolower(*s1)) && i == tolower(*s2))
		s1++, s2++;

	return ((unsigned char) tolower(*s1) - (unsigned char) tolower(*s2));
}
