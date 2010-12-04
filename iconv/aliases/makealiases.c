/**
 * IANA charset data to Iconv Aliases file convertor
 *
 * Version history:
 *
 * 0.01 - Initial version
 * 0.02 - Added "utf8" alias seen in the wild
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct extra {
	const char *canon;
	const char *aliases;
} extras[] = {
	{ "ISO-8859-1", "8859_1 ISO8859-1" },
	{ "ISO-8859-2", "8859_2 ISO8859-2" },
	{ "ISO-8859-3", "8859_3 ISO8859-3" },
	{ "ISO-8859-4", "8859_4 ISO8859-4" },
	{ "ISO-8859-5", "8859_5 ISO8859-5" },
	{ "ISO-8859-7", "8859_7 ISO8859-7" },
	{ "ISO-8859-8", "8859_8 ISO8859-8" },
	{ "ISO-8859-9", "8859_9 ISO8859-9" },
	{ "ISO-8859-10", "8859_10 ISO8859-10" },
	{ "ISO-8859-13", "8859_13 ISO8859-13" },
	{ "ISO-8859-14", "8859_14 ISO8859-14" },
	{ "ISO-8859-15", "8859_15 ISO8859-15" },
	{ "Shift_JIS", "X-SJIS Shift-JIS" },
	{ "EUC-JP", "EUCJP" },
	{ "EUC-KR", "EUCKR" },
	{ "UTF-8", "UNICODE-1-1-UTF-8 UNICODE-2-0-UTF-8 utf8" },
	{ "ISO-10646-UCS-4", "UCS-4 UCS4" },
	{ "ISO-10646-UCS-2", "UCS-2 UCS2" },
	{ "GB2312", "EUC-CN EUCCN CN-GB" },
	{ "Big5", "BIG-FIVE BIG-5 CN-BIG5 BIG_FIVE x-x-big5" },
	{ "macintosh", "MACROMAN MAC-ROMAN X-MAC-ROMAN" },
	{ "windows-1250", "CP1250 MS-EE" },
	{ "windows-1251", "CP1251 MS-CYRL" },
	{ "windows-1252", "CP1252 MS-ANSI" },
	{ "windows-1253", "CP1253 MS-GREEK" },
	{ "windows-1254", "CP1254 MS-TURK" },
	{ "windows-1256", "CP1256 MS-ARAB" },
	{ "windows-1257", "CP1257 WINBALTRIM" },
};
#define EXTRAS_SIZE (sizeof(extras) / sizeof(extras[0]))

/*
 * Make aliases file from IANA charset data.
 * The canonical name of an encoding is that which follows the "Name:" tag
 * in the input file. There is an exception, however, for those encodings
 * which have an alias which is denoted as the "preferred MIME name". For
 * these encodings, the preferred MIME name is taken as the canonical form.
 */

#define TOP      argv[1]
#define SETS     argv[2]
#define BOTTOM   argv[3]
#define ALIASES  argv[4]

int main(int argc, const char **argv)
{
	FILE *in, *out;
	char buf[200], name[64];
	short mibenum;
	char *s, *n, *aliases, *temp;
	int i;
	int namelen;

	in = fopen(TOP, "r");
	if (!in)
		return 1;

	out = fopen(ALIASES, "w");
	if (!out)
		return 1;

	while (fgets(buf, sizeof buf, in)) {
		fputs(buf, out);
	}

	fclose(in);

	in = fopen(SETS, "r");
	if (!in) {
		fclose(out);
		return 1;
	}

	fgets(buf, sizeof buf, in);

	while (1) {
		/* find start of record */
		if (strncmp(buf, "Name:", 5) != 0) {
			while(fgets(buf, sizeof buf, in)) {
				if (strncmp(buf, "Name:", 5) == 0)
					break;
			}
		}
		if(strncmp(buf, "Name:", 5) != 0)
			break;

		buf[strlen(buf) - 1] = '\0';

		s = buf+5;
		/* skip whitespace */
		while (isspace(*s))
			s++;
		/* copy name to buffer */
		n = name;
		while (*s) {
			if (isspace(*s))
				break;
			*n++ = *s++;
		}
		*n = '\0';

		/* get mibenum */
		while(fgets(buf, sizeof buf, in)) {
			if (strncmp(buf, "Name:", 5) == 0)
				break;
			if (strncmp(buf, "MIBenum:", 8) == 0)
				break;
		}
		if (strncmp(buf, "MIBenum:", 8) != 0)
			continue;

		buf[strlen(buf) - 1] = '\0';

		s = buf+8;
		while (isspace(*s))
			s++;
		mibenum = atoi(s);

		aliases = malloc(1);
		if (!aliases)
			break;
		*aliases = '\0';

		/* parse aliases */
		while(fgets(buf, sizeof buf, in)) {
			if (strncmp(buf, "Name:", 5) == 0)
				break;
			if (strncmp(buf, "Alias:", 6) != 0)
				continue;

			buf[strlen(buf) - 1] = '\0';

			s = buf + 6;
			while (isspace(*s))
				s++;

			if (strncmp(s, "None", 4) == 0)
				/* ignore this */
				continue;

			if (strstr(s, "preferred MIME name") != 0) {
				temp = realloc(aliases,
						strlen(aliases) + 1 +
						strlen(name) + 1);
				if (!temp)
					goto end;
				aliases = temp;
				sprintf(aliases, "%s%s%s", aliases,
					aliases[0] == '\0' ? "" : " ", name);
				n = name;
				while (*s) {
					if (isspace(*s))
						break;
					*n++ = *s++;
				}
				*n = '\0';
			}
			else {
				n = s;
				while (*n) {
					if (isspace(*n))
						break;
					n++;
				}
				temp = realloc(aliases,
					strlen(aliases) + 1 + (n - s) + 1);
				if (!temp)
					goto end;
				aliases = temp;
				n = aliases + strlen(aliases);
				if (aliases[0] != '\0')
					*n++ = ' ';
				while (*s) {
					if (isspace(*s))
						break;
					*n++ = *s++;
				}
				*n = '\0';
			}
		}

		fprintf(out, "%s\t", name);

		/* Rounded up to tab stop */
		namelen = (strlen(name) + 8) & ~(8 - 1);
		while (namelen < 3 * 8) {
			fputc('\t', out);
			namelen += 8;
		}

		fprintf(out, "%d", mibenum);

		if (aliases[0] != '\0')
			fprintf(out, "\t\t%s", aliases);
		for (i = 0; i != EXTRAS_SIZE; i++) {
			if (strcmp(name, extras[i].canon) == 0) {
				fprintf(out, "%s%s",
					aliases[0] == '\0' ? "\t\t" : " ",
					extras[i].aliases);
				break;
			}
		}
		fprintf(out, "\n");

		free(aliases);
	}

end:
	fclose(in);

	in = fopen(BOTTOM, "r");
	if (!in) {
		fclose(out);
		return 1;
	}

	while (fgets(buf, sizeof buf, in))
		fputs(buf, out);

	fclose(in);
	fclose(out);

	return 0;
}

