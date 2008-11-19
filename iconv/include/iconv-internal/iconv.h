#ifndef _LIB_ICONV_INTERNAL_H
#define _LIB_ICONV_INTERNAL_H

#include <iconv/iconv.h>

/*
 * Initialise the iconv library
 */
int iconv_initialise(const char *aliases_file);

/*
 * Finalise the iconv library
 */
void iconv_finalise(void);

#endif

