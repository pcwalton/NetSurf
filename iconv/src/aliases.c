#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"

struct alias {
	struct alias *next;
	struct canon *canon;
	unsigned short name_len;
	char name[1];
};

#define HASH_SIZE (43)
static struct canon *canon_tab[HASH_SIZE];
static struct alias *alias_tab[HASH_SIZE];

static bool create_alias(const char *alias, struct canon *c);
static struct canon *create_canon(const char *canon, short mibenum);
static int hash_val(const char *alias);

#ifdef TEST
int main (void)
{
	struct canon *c;

	create_alias_data("Unicode:Files.Aliases");

	dump_alias_data();

	c = alias_canonicalise("moose");
	if (c)
		printf("!!!\n");

	c = alias_canonicalise("csinvariant");
	if (c)
		printf("%s %d\n", c->name, c->mib_enum);

	c = alias_canonicalise("nats-sefi-add");
	if (c)
		printf("%s %d\n", c->name, c->mib_enum);

	printf("%d\n", mibenum_from_name(c->name));

	printf("%s\n", mibenum_to_name(c->mib_enum));

	free_alias_data();

	return 0;
}
#endif

/**
 * Create an alias
 *
 * \param alias The alias name
 * \param c The canonical form
 * \return true on success, false otherwise
 */
bool create_alias(const char *alias, struct canon *c)
{
	struct alias *a;
	int hash;

	if (!alias || !c)
		return false;

	a = malloc(sizeof(struct alias) + strlen(alias) + 1);
	if (!a)
		return false;

	a->canon = c;
	a->name_len = strlen(alias);
	strcpy(a->name, alias);
	a->name[a->name_len] = '\0';

	hash = hash_val(alias);

	a->next = alias_tab[hash];
	alias_tab[hash] = a;

	return true;
}

/**
 * Create a canonical form
 *
 * \param canon The canonical name
 * \param mibenum The MIB enum value
 * \return Pointer to struct canon or NULL on error
 */
struct canon *create_canon(const char *canon, short mibenum)
{
	struct canon *c;
	int hash, len;

	if (!canon)
		return NULL;

	len = strlen(canon);

	c = malloc(sizeof(struct canon) + len + 1);
	if (!c)
		return NULL;

	c->mib_enum = mibenum;
	c->name_len = len;
	strcpy(c->name, canon);
	c->name[len] = '\0';

	hash = hash_val(canon);

	c->next = canon_tab[hash];
	canon_tab[hash] = c;

	return c;
}

/**
 * Hash function
 *
 * \param alias String to hash
 * \return The hashed value
 */
int hash_val(const char *alias)
{
	const char *s = alias;
	unsigned int h = 5381;

	if (!alias)
		return 0;

	while (*s)
		h = (h * 33) ^ (*s++ & ~0x20); /* case insensitive */

	return h % HASH_SIZE;
}

/**
 * Free all alias data
 */
void free_alias_data(void)
{
	struct canon *c, *d;
	struct alias *a, *b;
	int i;

	for (i = 0; i != HASH_SIZE; i++) {
		for (c = canon_tab[i]; c; c = d) {
			d = c->next;
			free(c);
		}
		canon_tab[i] = NULL;

		for (a = alias_tab[i]; a; a = b) {
			b = a->next;
			free(a);
		}
		alias_tab[i] = NULL;
	}
}

/**
 * Dump all alias data to stdout
 */
void dump_alias_data(void)
{
	struct canon *c;
	struct alias *a;
	int i;
	size_t size = 0;

	for (i = 0; i != HASH_SIZE; i++) {
		for (c = canon_tab[i]; c; c = c->next) {
			printf("%s\n", c->name);
			size += offsetof(struct canon, name) + c->name_len;
		}

		for (a = alias_tab[i]; a; a = a->next) {
			printf("%s\n", a->name);
			size += offsetof(struct alias, name) + a->name_len;
		}
	}

#ifdef TEST
	size += (sizeof(canon_tab) / sizeof(canon_tab[0]));
	size += (sizeof(alias_tab) / sizeof(alias_tab[0]));

	printf("%d\n", size);
#endif
}

/**
 * Create alias data from Aliases file
 *
 * \param filename  The path to the Aliases file
 * \return 1 on success, 0 on failure.
 */
int create_alias_data(const char *filename)
{
	char buf[300];
	FILE *fp;

	if (!filename)
		return 0;

	fp = fopen(filename, "r");
	if (!fp)
		return 0;

	while (fgets(buf, sizeof buf, fp)) {
		char *p, *aliases = 0, *mib, *end;
		struct canon *cf;

		if (buf[0] == 0 || buf[0] == '#')
			/* skip blank lines or comments */
			continue;

		buf[strlen(buf) - 1] = 0; /* lose terminating newline */
		end = buf + strlen(buf);

		/* find end of canonical form */
		for (p = buf; *p && !isspace(*p) && !iscntrl(*p); p++)
			; /* do nothing */
		if (p >= end)
			continue;
		*p++ = '\0'; /* terminate canonical form */

		/* skip whitespace */
		for (; *p && isspace(*p); p++)
			; /* do nothing */
		if (p >= end)
			continue;
		mib = p;

		/* find end of mibenum */
		for (; *p && !isspace(*p) && !iscntrl(*p); p++)
			; /* do nothing */
		if (p < end)
			*p++ = '\0'; /* terminate mibenum */

		cf = create_canon(buf, atoi(mib));
		if (!cf)
			continue;

		/* skip whitespace */
		for (; p < end && *p && isspace(*p); p++)
			; /* do nothing */
		if (p >= end)
			continue;
		aliases = p;

		while (p < end) {
			/* find end of alias */
			for (; *p && !isspace(*p) && !iscntrl(*p); p++)
				; /* do nothing */
			if (p > end)
				/* stop if we've gone past the end */
				break;
			/* terminate current alias */
			*p++ = '\0';

			if (!create_alias(aliases, cf))
				break;

			/* in terminating, we may have advanced
			 * past the end - check this here */
			if (p >= end)
				break;

			/* skip whitespace */
			for (; *p && isspace(*p); p++)
				; /* do nothing */

			if (p >= end)
				/* gone past end => stop */
				break;

			/* update pointer to current alias */
			aliases = p;
		}
	}

	fclose(fp);

	return 1;
}

/**
 * Retrieve the canonical form of an alias name
 *
 * \param alias The alias name
 * \return Pointer to struct canon or NULL if not found
 */
struct canon *alias_canonicalise(const char *alias)
{
	int hash, len;
	struct canon *c;
	struct alias *a;

	if (!alias)
		return NULL;

	hash = hash_val(alias);
	len = strlen(alias);

	for (c = canon_tab[hash]; c; c = c->next)
		if (c->name_len == len && strcasecmp(c->name, alias) == 0)
			break;
	if (c)
		return c;

	for (a = alias_tab[hash]; a; a = a->next)
		if (a->name_len == len && strcasecmp(a->name, alias) == 0)
			break;
	if (a)
		return a->canon;

	return NULL;
}

/**
 * Retrieve the MIB enum value assigned to an encoding name
 *
 * \param alias The alias to lookup
 * \return The MIB enum value, or 0 if not found
 */
short mibenum_from_name(const char *alias)
{
	struct canon *c;

	if (!alias)
		return 0;

	c = alias_canonicalise(alias);
	if (!c)
		return 0;

	return c->mib_enum;
}

/**
 * Retrieve the canonical name of an encoding from the MIB enum
 *
 * \param mibenum The MIB enum value
 * \return Pointer to canonical name, or NULL if not found
 */
const char *mibenum_to_name(short mibenum)
{
	int i;
	struct canon *c;

	for (i = 0; i != HASH_SIZE; i++)
		for (c = canon_tab[i]; c; c = c->next)
			if (c->mib_enum == mibenum)
				return c->name;

	return NULL;
}
