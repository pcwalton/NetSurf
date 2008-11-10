/* iconv implementation - see iconv.h for docs */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/errno.h>

#include <unicode/charsets.h>
#include <unicode/encoding.h>

#include <iconv/iconv.h>

#include "internal.h"

static struct encoding_context *context_list;

static int character_callback(void *handle, UCS4 c);
static void parse_parameters(struct encoding_context *e, const char *params,
		bool destination);
static void parse_parameter(struct encoding_context *e, const char *param,
		int length, bool destination);

int iconv_initialise(const char *aliases_file)
{
	if (aliases_file == NULL)
		return false;

	if (create_alias_data(aliases_file) == false)
		return false;

	encoding_initialise();

	return true;
}

void iconv_finalise(void)
{
	struct encoding_context *a, *b;

	/* clients may quit / die without cleaning up. */
	for (a = context_list; a; a = b) {
		b = a->next;
		if (a->in)
			encoding_delete(a->in);
		if (a->out)
			encoding_delete(a->out);
		iconv_eightbit_delete(a);
		free(a);
	}

	free_alias_data();

	/* finalise the unicode library */
	encoding_tidyup();
}

iconv_t iconv_open(const char *tocode, const char *fromcode)
{
	int to = 0, from = 0;
	struct encoding_context *e;
	struct canon *c;
	bool to_force_le = false, from_force_le = false;
	char totemp[128], fromtemp[128];
	const char *slash;
	unsigned int len;

	/* can't do anything without these */
	if (!tocode || !fromcode) {
		errno = EINVAL;
		return (iconv_t)(-1);
	}

	e = calloc(1, sizeof(*e));
	if (!e) {
		LOG(("malloc failed"));
		errno = ENOMEM;
		return (iconv_t)(-1);
	}

	/* strip any parameters off the end of the tocode string */
	slash = strchr(tocode, '/');
	len = slash ? (unsigned) (slash - tocode) : strlen(tocode);
	snprintf(totemp, sizeof totemp, "%.*s", len, tocode);

	/* parse parameters */
	if (slash && *(slash + 1) == '/' && *(slash + 2) != '\0')
		parse_parameters(e, slash + 2, true);

	/* strip any parameters off the end of the fromcode string */
	slash = strchr(fromcode, '/');
	len = slash ? (unsigned) (slash - fromcode) : strlen(fromcode);
	snprintf(fromtemp, sizeof fromtemp, "%.*s", len, fromcode);

	/* parse parameters */
	if (slash && *(slash + 1) == '/' && *(slash + 2) != '\0')
		parse_parameters(e, slash + 2, false);

	/* try our own 8bit charset code first */
	to = iconv_eightbit_number_from_name(totemp);
	from = iconv_eightbit_number_from_name(fromtemp);

	/* if that failed, try the UnicodeLib functionality */
	if (!to)
		to = iconv_encoding_number_from_name(totemp);

	if (!from)
		from = iconv_encoding_number_from_name(fromtemp);

	/* if that failed, perhaps it was an endian-specific variant of
	 * something UnicodeLib can handle? */
	if (!to) {
		c = alias_canonicalise(totemp);
		if (c) {
			switch(c->mib_enum) {
			case 1013: /* UTF-16BE */
				to = csUnicode11;
				break;
			case 1014: /* UTF-16LE */
				to = csUnicode11;
				to_force_le = true;
				break;
			case 1018: /* UTF-32BE */
				to = csUCS4;
				break;
			case 1019: /* UTF-32LE */
				to = csUCS4;
				to_force_le = true;
				break;
			}
		}
	}

	if (!from) {
		c = alias_canonicalise(fromtemp);
		if (c) {
			switch(c->mib_enum) {
			case 1013: /* UTF-16BE */
				from = csUnicode11;
				break;
			case 1014: /* UTF-16LE */
				from = csUnicode11;
				from_force_le = true;
				break;
			case 1018: /* UTF-32BE */
				from = csUCS4;
				break;
			case 1019: /* UTF-32LE */
				from = csUCS4;
				from_force_le = true;
				break;
			}
		}
	}

	LOG(("to: %d(%s) from: %d(%s)", to, totemp, from, fromtemp));

	/* ensure both encodings are recognised */
	if (to == 0 || from == 0) {
		free(e);
		errno = EINVAL;
		return (iconv_t)(-1);
	}

	/* bit 30 set indicates that this is an 8bit encoding */
	if (from & (1<<30))
		e->intab = iconv_eightbit_new(from & ~(1<<30));
	else {
		e->in = encoding_new(from, encoding_READ);
		if (e->in) {
			/* Set encoding flags */
			unsigned int flags = 0;
			if (from_force_le)
				flags |= encoding_FLAG_LITTLE_ENDIAN;

			c = alias_canonicalise(fromtemp);
			if (c && (c->mib_enum == csUCS4 ||
					c->mib_enum == csUnicode))
				flags |= encoding_FLAG_NO_HEADER;

			encoding_set_flags(e->in, flags, flags);
		}
	}

	/* neither created => memory error or somesuch. assume ENOMEM */
	/* no table is ever generated for ASCII */
	if (!e->in && !e->intab && (from & ~(1<<30)) != csASCII) {
		free(e);
		errno = ENOMEM;
		return (iconv_t)(-1);
	}

	if (to & (1<<30))
		e->outtab = iconv_eightbit_new(to & ~(1<<30));
	else {
		e->out = encoding_new(to, encoding_WRITE_STRICT);
		if (e->out) {
			/* Set encoding flags */
			unsigned int flags = 0;
			if (to_force_le)
				flags |= encoding_FLAG_LITTLE_ENDIAN;

			c = alias_canonicalise(totemp);
			if (c && (c->mib_enum == csUCS4 ||
					c->mib_enum == csUnicode))
				flags |= encoding_FLAG_NO_HEADER;

			encoding_set_flags(e->out, flags, flags);
		}
	}

	/* neither created => ENOMEM */
	if (!e->out && !e->outtab && (to & ~(1<<30)) != csASCII) {
		if (e->in)
			encoding_delete(e->in);
		iconv_eightbit_delete(e);
		free(e);
		errno = ENOMEM;
		return (iconv_t)(-1);
	}

	/* add to list */
	e->prev = 0;
	e->next = context_list;
	if (context_list)
		context_list->prev = e;
	context_list = e;

	return (iconv_t)e;
}

size_t iconv(iconv_t cd, char **inbuf, size_t *inbytesleft, char **outbuf,
		size_t *outbytesleft)
{
	struct encoding_context *e;
	unsigned read;

	/* search for cd in list */
	for (e = context_list; e; e = e->next)
		if (e == (struct encoding_context *)cd)
			break;

	/* not found => invalid */
	if (!e) {
		errno = EINVAL;
		return (size_t)(-1);
	}

	if (inbuf == NULL || *inbuf == NULL) {
		if (e->in)
			encoding_reset(e->in);
		return 0;
	}

	/* Is there any point doing anything? */
	if (!outbuf || !(*outbuf) || !outbytesleft) {
		errno = EINVAL;
		return (size_t)(-1);
	}

	e->outbuf = outbuf;
	e->outbytesleft = outbytesleft;

	LOG(("reading"));

	if (e->in)
		read = encoding_read(e->in, character_callback, *inbuf,
				*inbytesleft, e);
	else
		read = iconv_eightbit_read(e, character_callback, *inbuf,
				*inbytesleft, e);

	LOG(("done"));

	LOG(("read: %d, ibl: %d, obl: %d", read, *inbytesleft, *outbytesleft));

	/* 2 */
	if (read == *inbytesleft) {
		*inbuf += read;
		*inbytesleft = 0;
		return 0;
	}
	/* 4 */
	else if ((int)*outbytesleft < 0) {
		LOG(("e2big"));
		*outbytesleft = 0;
		*inbuf += read - 1;
		*inbytesleft -= read - 1;
		errno = E2BIG;
	}
	/** \todo find a mechanism for distinguishing between 1 & 3 */
	/* 1 */
	else if (read != *inbytesleft) {
		*inbuf += read;
		*inbytesleft -= read;
		LOG(("eilseq"));
		errno = EILSEQ;
	}
	/* 3 */
	else if ((int)*outbytesleft >= 0) {
		*inbuf += read;
		*inbytesleft -= read;
		LOG(("einval"));
		errno = EINVAL;
	}

	LOG(("errno: %d", errno));

	return (size_t)(-1);
}

int iconv_close(iconv_t cd)
{
	struct encoding_context *e;

	/* search for cd in list */
	for (e = context_list; e; e = e->next)
		if (e == (struct encoding_context *)cd)
			break;

	/* not found => invalid */
	if (!e)
		return 0;

	if (e->in)
		encoding_delete(e->in);
	if (e->out)
		encoding_delete(e->out);
	iconv_eightbit_delete(e);

	/* remove from list */
	if (e->next)
		e->next->prev = e->prev;
	if (e->prev)
		e->prev->next = e->next;
	else
		context_list = e->next;

	free(e);

	/* reduce our memory usage somewhat */
	encoding_table_remove_unused(8 /* recommended value */);

	return 0;
}

/* this is called for each converted character */
int character_callback(void *handle, UCS4 c)
{
	struct encoding_context *e;
	int ret;

	e = (struct encoding_context*)handle;

	LOG(("outbuf: %p, free: %d", *e->outbuf, *e->outbytesleft));
	LOG(("writing: %d", c));

	if (e->out) {
		char *prev_outbuf = *e->outbuf;
		size_t prev_outbytesleft = *e->outbytesleft;

		ret = encoding_write(e->out, c, e->outbuf,
				(int*)e->outbytesleft);

		LOG(("ret: %d", ret));

		/* Why the need for this nonsense? UnicodeLib appears to
		 * decrease the count of free space in the buffer even
		 * if it doesn't write into it. This is a bug, as the
		 * documentation says that the buffer pointer AND free
		 * space count are left unmodified if nothing is written.
		 * Therefore, we have this hack until UnicodeLib gets fixed.
		 */
		if (ret == -1) {
			*e->outbytesleft = prev_outbytesleft -
					(*e->outbuf - prev_outbuf);
		}
	} else {
		ret = iconv_eightbit_write(e, c, e->outbuf,
				(int*)e->outbytesleft);
	}

	if (ret == -1) {
		/* Transliterate, if we've been asked to.
		 * Assumes that output is 8bit/8bit multibyte with ASCII G0.
		 * This should be fine as the only <>8bit encodings are
		 * UCS{2,4}, UTF-{16,32}, neither of which return -1.
		 * Also, afaiaa, all supported multibyte encodings are ASCII
		 * compatible. */
		/** \todo Actually perform some kind of transliteration */
		if (e->transliterate && (int)*e->outbytesleft > 0) {
			if (e->out) {
				/* Reset encoding write state */
				/** \todo this is a bit dodgy, as we only
				 * really need to ensure that the ASCII set
				 * is mapped into G0 in ISO2022 encodings.
				 * This will reset G1->G3, too, which may
				 * break things. If so, we may have to
				 * perform some dirty hackery which relies
				 * upon knowledge of UnicodeLib's internals
				 */
				encoding_write(e->out, NULL_UCS4, e->outbuf,
						(int*)e->outbytesleft);
			}

			if ((int)*e->outbytesleft > 0) {
				*(*e->outbuf)++ = '?';
				--*e->outbytesleft;

				ret = 1;
			} else {
				ret = 0;
			}
		} else {
			ret = 1;
		}
	}

	return (!ret);
}

void parse_parameters(struct encoding_context *e, const char *params,
		bool destination)
{
	char *slash = NULL, *prev = NULL;
	int len;

	len = strlen(params);

	while (slash - params < len &&
			(slash = strchr(params, '/')) != NULL) {
		parse_parameter(e, prev == NULL ? params : prev,
				slash - (prev == NULL ? params : prev),
				destination);

		prev = slash + 2;
		slash += 2;
	}

	if (slash == NULL)
		parse_parameter(e, prev == NULL ? params : prev,
				(params + len) -
					(prev == NULL ? params : prev),
				destination);
}

void parse_parameter(struct encoding_context *e, const char *param,
		int length, bool destination)
{
	if (length == 8 && strncasecmp(param, "TRANSLIT", 8) == 0) {
		if (destination)
			e->transliterate = 1;
	}
}

