#ifndef iconv_module_h_
#define iconv_module_h_

/* Ugh. Include libiconv internal header so we get access to aliases stuff */
#include "internal.h"

/* in menu.c */
size_t iconv_createmenu(size_t flags, char *buf, size_t buflen,
		const char *selected, char *data, size_t *dlen);
size_t iconv_decodemenu(size_t flags, void *menu, int *selections,
		char *buf, size_t buflen);

#endif

