/* Iconv stubs */

#include <errno.h>
#include <stdlib.h>

#include "kernel.h"
#include "swis.h"

#include "iconv.h"

/* SWI numbers */
#define Iconv_Open                (0x57540)
#define Iconv_Iconv               (0x57541)
#define Iconv_Close               (0x57542)
#define Iconv_Convert             (0x57543)
#define Iconv_CreateMenu          (0x57544)
#define Iconv_DecodeMenu          (0x57545)

/* Error numbers */
#define ERROR_BASE 0x81b900

#define ICONV_NOMEM (ERROR_BASE+0)
#define ICONV_INVAL (ERROR_BASE+1)
#define ICONV_2BIG  (ERROR_BASE+2)
#define ICONV_ILSEQ (ERROR_BASE+3)

iconv_t iconv_open(const char *tocode, const char *fromcode)
{
	iconv_t ret;
	_kernel_oserror *error;

	error = _swix(Iconv_Open, _INR(0,1) | _OUT(0), 
			tocode, fromcode, 
			&ret);
	if (error) {
		switch (error->errnum) {
			case ICONV_NOMEM:
				errno = ENOMEM;
				break;
			case ICONV_INVAL:
				errno = EINVAL;
				break;
			case ICONV_2BIG:
				errno = E2BIG;
				break;
			case ICONV_ILSEQ:
				errno = EILSEQ;
				break;
			default:
				errno = EINVAL; /* munge BAD_SWI to EINVAL */
				break;
		}
		return (iconv_t)(-1);
	}

	return ret;
}

size_t iconv(iconv_t cd, char **inbuf, size_t *inbytesleft, char **outbuf,
		size_t *outbytesleft)
{
	size_t ret;
	_kernel_oserror *error;

	error = _swix(Iconv_Iconv, _INR(0,4) | _OUT(0), 
			cd, inbuf, inbytesleft, outbuf, outbytesleft, 
			&ret);
	if (error) {
		switch (error->errnum) {
			case ICONV_NOMEM:
				errno = ENOMEM;
				break;
			case ICONV_INVAL:
				errno = EINVAL;
				break;
			case ICONV_2BIG:
				errno = E2BIG;
				break;
			case ICONV_ILSEQ:
				errno = EILSEQ;
				break;
			default:
				errno = EINVAL; /* munge BAD_SWI to EINVAL */
				break;
		}
		return (size_t)(-1);
	}

	return ret;
}

int iconv_close(iconv_t cd)
{
	int ret;
	_kernel_oserror *error;

	error = _swix(Iconv_Close, _IN(0) | _OUT(0), 
			cd, 
			&ret);
	if (error) {
		switch (error->errnum) {
			case ICONV_NOMEM:
				errno = ENOMEM;
				break;
			case ICONV_INVAL:
				errno = EINVAL;
				break;
			case ICONV_2BIG:
				errno = E2BIG;
				break;
			case ICONV_ILSEQ:
				errno = EILSEQ;
				break;
			default:
				errno = EINVAL; /* munge BAD_SWI to EINVAL */
				break;
		}
		return -1;
	}

	return ret;
}
