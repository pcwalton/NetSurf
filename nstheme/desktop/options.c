/*
 * This file is part of NetSurf, http://netsurf.sourceforge.net/
 * Licensed under the GNU General Public License,
 *                http://www.opensource.org/licenses/gpl-license
 * Copyright 2003 Phil Mellor <monkeyson@users.sourceforge.net>
 * Copyright 2003 John M Bell <jmb202@ecs.soton.ac.uk>
 * Copyright 2004 James Bursa <bursa@users.sourceforge.net>
 * Copyright 2004 Richard Wilson <not_ginger_matt@users.sourceforge.net>
 */

/** \file
 * Option reading and saving (implementation).
 *
 * Options are stored in the format key:value, one per line. For bool options,
 * value is "0" or "1".
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "nstheme/desktop/options.h"
#include "nstheme/utils/log.h"
#include "nstheme/utils/utils.h"

#ifdef riscos
#include "nstheme/riscos/options.h"
#else
#define EXTRA_OPTION_DEFINE
#define EXTRA_OPTION_TABLE
#endif


EXTRA_OPTION_DEFINE


struct {
	const char *key;
	enum { OPTION_BOOL, OPTION_INTEGER, OPTION_STRING } type;
	void *p;
} option_table[] = {
	EXTRA_OPTION_TABLE
};

#define option_table_entries (sizeof option_table / sizeof option_table[0])


/**
 * Read options from a file.
 *
 * \param  path  name of file to read options from
 *
 * Option variables corresponding to lines in the file are updated. Missing
 * options are unchanged. If the file fails to open, options are unchanged.
 */

void options_read(const char *path)
{
	char s[100];
	FILE *fp;

	fp = fopen(path, "r");
	if (!fp) {
		LOG(("failed to open file '%s'", path));
		return;
	}

	while (fgets(s, 100, fp)) {
		char *colon, *value;
		unsigned int i;

		if (s[0] == 0 || s[0] == '#')
			continue;
		colon = strchr(s, ':');
		if (colon == 0)
			continue;
		s[strlen(s) - 1] = 0;  /* remove \n at end */
		*colon = 0;  /* terminate key */
		value = colon + 1;

		for (i = 0; i != option_table_entries; i++) {
			if (strcasecmp(s, option_table[i].key) != 0)
				continue;

			switch (option_table[i].type) {
				case OPTION_BOOL:
					*((bool *) option_table[i].p) =
							value[0] == '1';
					break;

				case OPTION_INTEGER:
					*((int *) option_table[i].p) =
							atoi(value);
					break;

				case OPTION_STRING:
					free(*((char **) option_table[i].p));
					*((char **) option_table[i].p) =
							strdup(value);
					break;
			}
			break;
		}
	}

	fclose(fp);
}


/**
 * Save options to a file.
 *
 * \param  path  name of file to write options to
 *
 * Errors are ignored.
 */

void options_write(const char *path)
{
	unsigned int i;
	FILE *fp;

	fp = fopen(path, "w");
	if (!fp) {
		LOG(("failed to open file '%s' for writing", path));
		return;
	}

	for (i = 0; i != option_table_entries; i++) {
		fprintf(fp, "%s:", option_table[i].key);
		switch (option_table[i].type) {
			case OPTION_BOOL:
				fprintf(fp, "%c", *((bool *) option_table[i].p) ?
						'1' : '0');
				break;

			case OPTION_INTEGER:
				fprintf(fp, "%i", *((int *) option_table[i].p));
				break;

			case OPTION_STRING:
				if (*((char **) option_table[i].p))
					fprintf(fp, "%s", *((char **) option_table[i].p));
				break;
		}
		fprintf(fp, "\n");
        }

	fclose(fp);
}
