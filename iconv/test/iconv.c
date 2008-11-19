#include <stdio.h>

#include <iconv-internal/iconv.h>

#include "testutils.h"

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: %s <aliases_file>\n", argv[0]);
		return 1;
	}

	assert(iconv_initialise(argv[1]) == 1);

	iconv_finalise();

	printf("PASS\n");

	return 0;
}

