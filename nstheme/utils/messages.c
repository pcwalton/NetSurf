/*
 * This file is part of NetSurf, http://netsurf.sourceforge.net/
 * Licensed under the GNU General Public License,
 *                http://www.opensource.org/licenses/gpl-license
 * Copyright 2004 James Bursa <bursa@users.sourceforge.net>
 */

/** \file
 * Localised message support (implementation).
 *
 * Native language messages are loaded from a file and stored hashed by key for
 * fast access.
 */

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "nstheme/utils/log.h"
#include "nstheme/utils/messages.h"
#include "nstheme/utils/utils.h"

/** We store the messages in a fixed-size hash table. */
#define HASH_SIZE 77

/** Maximum length of a key. */
#define MAX_KEY_LENGTH 24

/** Entry in the messages hash table. */
struct messages_entry {
	struct messages_entry *next;  /**< Next entry in this hash chain. */
	char key[MAX_KEY_LENGTH];
	char value[1];
};

/** Localised messages hash table. */
static struct messages_entry *messages_table[HASH_SIZE];


static unsigned int messages_hash(const char *s);


/**
 * Read keys and values from messages file.
 *
 * \param  path  pathname of messages file
 *
 * The messages are merged with any previously loaded messages. Any keys which
 * are present already are replaced with the new value.
 *
 * Exits through die() in case of error.
 */

void messages_load(const char *path)
{
	char s[300];
	FILE *fp;

	fp = fopen(path, "r");
	if (!fp) {
		snprintf(s, sizeof s, "Unable to open messages file "
				"\"%.100s\": %s", path, strerror(errno));
		s[sizeof s - 1] = 0;
		LOG(("%s", s));
		die(s);
	}

	while (fgets(s, sizeof s, fp)) {
		char *colon, *value;
		unsigned int slot;
		struct messages_entry *entry;
		size_t length;

		if (s[0] == 0 || s[0] == '#')
			continue;

		s[strlen(s) - 1] = 0;  /* remove \n at end */
		colon = strchr(s, ':');
		if (!colon)
			continue;
		*colon = 0;  /* terminate key */
		value = colon + 1;
		length = strlen(value);

		entry = malloc(sizeof *entry + length + 1);
		if (!entry) {
			snprintf(s, sizeof s, "Not enough memory to load "
					"messages file \"%.100s\".", path);
			s[sizeof s - 1] = 0;
			LOG(("%s", s));
			die(s);
		}
		strncpy(entry->key, s, MAX_KEY_LENGTH);
		strcpy(entry->value, value);
		slot = messages_hash(entry->key);
		entry->next = messages_table[slot];
		messages_table[slot] = entry;
	}

	fclose(fp);
}


/**
 * Fast lookup of a message by key.
 *
 * \param  key  key of message
 * \return  value of message, or key if not found
 */

const char *messages_get(const char *key)
{
	struct messages_entry *entry;

	for (entry = messages_table[messages_hash(key)];
			entry && strcasecmp(entry->key, key) != 0;
			entry = entry->next)
		;
	if (!entry)
		return key;
	return entry->value;
}

/**
 * Retrieve the key associated with a value
 *
 * \param value The value as returned by messages_get
 * \return The key associated with the value or NULL if not found
 */
const char *messages_get_key(const char *value)
{
	const char *key = value - MAX_KEY_LENGTH;
	const char *temp_value = messages_get(key);

	if (strcmp(value, temp_value) == 0)
		return key;

	return NULL;
}


/**
 * Hash function for keys.
 */

unsigned int messages_hash(const char *s)
{
	unsigned int i, z = 0;
	if (!s)
		return 0;
	for (i = 0; i != MAX_KEY_LENGTH && s[i]; i++)
		z += s[i] & 0x1f;  /* lower 5 bits, case insensitive */
	return z % HASH_SIZE;
}


/**
 * Dump set of loaded messages.
 */

void messages_dump(void)
{
	unsigned int i;
	for (i = 0; i != HASH_SIZE; i++) {
		struct messages_entry *entry;
		for (entry = messages_table[i]; entry; entry = entry->next)
			printf("%.20s:%s\n", entry->key, entry->value);
	}
}
