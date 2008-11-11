/* Wrapper around module API so we can run/test it on non-RO machines */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"

#ifndef __riscos__

int main(int argc, char **argv)
{
	_kernel_oserror *error;
	char *buf = NULL;
	int buf_alloc = 0, buf_len = 0;

	for (int i = 1; i < argc; i++) {
		/* Need a leading space for all but the first argument */
		int req = buf_len + (buf_len > 0 ? 1 : 0) + strlen(argv[i]) + 1;

		while (req > buf_alloc) {
			char *temp = realloc(buf, buf_alloc + 4096);
			if (temp == NULL) {
				printf("Insufficient memory for parameters");
				return 1;
			}

			buf = temp;
			buf_alloc += 4096;
		}

		if (buf_len > 0) {
			memcpy(buf + buf_len, " ", 1);
			buf_len++;
		}

		memcpy(buf + buf_len, argv[i], strlen(argv[i]));

		buf_len += strlen(argv[i]);

		buf[buf_len] = '\0';
	}

	error = mod_init(NULL, 0, NULL);
	if (error != NULL) {
		free(buf);
		printf("mod_init: %s (%d)\n", error->errmess, error->errnum);
		return 1;
	}

	error = command_handler(buf, argc - 1, CMD_Iconv, NULL);
	if (error != NULL) {
		free(buf);
		printf("command_handler: %s (%d)\n", error->errmess, 
				error->errnum);
		return 1;
	}

	error = mod_fini(0, 0, NULL);
	if (error != NULL) {
		free(buf);
		printf("mod_fini: %s (%d)\n", error->errmess, error->errnum);
		return 1;
	}

	free(buf);

	return 0;
}

#endif

