/* Wrapper around module API so we can run/test it on non-RO machines */

#ifndef __riscos__

#include <stdio.h>

#include "header.h"

int main(int argc, char **argv)
{
	_kernel_oserror *error;

	if (argc != 1) {
		printf("Usage: %s\n", argv[0]);
		return 1;
	}

	error = mod_init(NULL, 0, NULL);
	if (error != NULL) {
		printf("mod_init: %s (%d)\n", error->errmess, error->errnum);
		return 1;
	}

	error = mod_fini(0, 0, NULL);
	if (error != NULL) {
		printf("mod_fini: %s (%d)\n", error->errmess, error->errnum);
		return 1;
	}

	return 0;
}

#endif

