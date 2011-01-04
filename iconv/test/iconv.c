#include <stdio.h>
#include <string.h>

#include <iconv-internal/iconv.h>

#include "testutils.h"

#ifdef __riscos__
#define ALIASES_FILE "Files.Aliases"
#else
#define ALIASES_FILE "Files/Aliases"
#endif

int main(int argc, char **argv)
{
	const char *ucpath;
	int alen;
	char aliases[4096];

	UNUSED(argc);
	UNUSED(argv);

#ifdef __riscos__
	ucpath = "Unicode:";
#else
	ucpath = getenv("UNICODE_DIR");
#endif

	assert(ucpath != NULL);

	strncpy(aliases, ucpath, sizeof(aliases));
	alen = strlen(aliases);
#ifndef __riscos__
	if (aliases[alen - 1] != '/') {
		strncat(aliases, "/", sizeof(aliases) - alen - 1);
		alen += 1;
	}
#endif
	strncat(aliases, ALIASES_FILE, sizeof(aliases) - alen - 1);
	aliases[sizeof(aliases) - 1] = '\0';

	assert(iconv_initialise(aliases) == 1);

	iconv_finalise();

	printf("PASS\n");

	return 0;
}

