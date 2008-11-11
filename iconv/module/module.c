/* Iconv module interface */

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <sys/errno.h>

#include <iconv/iconv.h>

#include "errors.h"
#include "header.h"
#include "module.h"

#ifdef __riscos__
#define ALIASES_FILE "Unicode:Files.Aliases"
#else
#define ALIASES_FILE "Aliases"
#endif

static _kernel_oserror ErrorGeneric = { 0x0, "" };

static size_t iconv_convert(_kernel_swi_regs *r);
static int errno_to_iconv_error(int num);

/* Module initialisation */
_kernel_oserror *mod_init(const char *tail, int podule_base, void *pw)
{
	UNUSED(tail);
	UNUSED(podule_base);
	UNUSED(pw);

	/* ensure the !Unicode resource exists */
	if (!getenv("Unicode$Path")) {
		strncpy(ErrorGeneric.errmess, "!Unicode resource not found.",
				252);
		return &ErrorGeneric;
	}

	if (iconv_initialise(ALIASES_FILE) == false) {
		strncpy(ErrorGeneric.errmess, "Unicode:Files.Aliases not "
				"found. Please read the Iconv installation "
				"instructions.", 252);
		return &ErrorGeneric;
	}

	return NULL;
}

/* Module finalisation */
_kernel_oserror *mod_fini(int fatal, int podule_base, void *pw)
{
	UNUSED(fatal);
	UNUSED(podule_base);
	UNUSED(pw);

	iconv_finalise();

	return NULL;
}

/* SWI handler */
_kernel_oserror *swi_handler(int swi_off, _kernel_swi_regs *regs, void *pw)
{
	intptr_t ret;

	UNUSED(pw);

	if (swi_off > 5)
		return error_BAD_SWI;

	switch (swi_off) {
		case 0: /* Iconv_Open */
			if ((ret = (intptr_t)
				iconv_open((const char*)regs->r[0],
					(const char*)regs->r[1])) == -1) {
				ErrorGeneric.errnum = 
						errno_to_iconv_error(errno);
				return &ErrorGeneric;
			}
			regs->r[0] = ret;
			break;
		case 1: /* Iconv_Iconv */
			if ((ret = (intptr_t)
				iconv((iconv_t)regs->r[0],
					(char**)regs->r[1],
					(size_t*)regs->r[2],
					(char**)regs->r[3],
					(size_t*)regs->r[4])) == -1) {
				ErrorGeneric.errnum = 
						errno_to_iconv_error(errno);
				return &ErrorGeneric;
			}
			regs->r[0] = ret;
			break;
		case 2: /* Iconv_Close */
			if ((ret = (intptr_t)
				iconv_close((iconv_t)regs->r[0])) == -1) {
				ErrorGeneric.errnum = 
						errno_to_iconv_error(errno);
				return &ErrorGeneric;
			}
			regs->r[0] = ret;
			break;
		case 3: /* Iconv_Convert */
			if ((ret = (intptr_t)
				iconv_convert(regs)) == -1) {
				ErrorGeneric.errnum = 
						errno_to_iconv_error(errno);
				return &ErrorGeneric;
			}
			regs->r[0] = ret;
			break;
		case 4: /* Iconv_CreateMenu */
			{
				size_t dlen = regs->r[5];
				regs->r[2] = iconv_createmenu(regs->r[0],
						(char *)regs->r[1],
						regs->r[2],
						(const char *)regs->r[3],
						(char *)regs->r[4],
						&dlen);
				regs->r[5] = dlen;
			}
			break;
		case 5: /* Iconv_DecodeMenu */
			regs->r[4] = iconv_decodemenu(regs->r[0],
					(void *)regs->r[1],
					(int *)regs->r[2],
					(char *)regs->r[3], regs->r[4]);
			break;
	}

	return NULL;
}

/* *command handler */
_kernel_oserror *command_handler(const char *arg_string, int argc,
		int cmd_no, void *pw)
{
	UNUSED(arg_string);
	UNUSED(argc);
	UNUSED(pw);

	switch (cmd_no) {
	case CMD_ReadAliases:
		free_alias_data();
		if (!create_alias_data(ALIASES_FILE)) {
			strcpy(ErrorGeneric.errmess,
					"Failed reading Aliases file.");
			return &ErrorGeneric;
		}
		break;
	default:
		break;
	}

	return NULL;
}

size_t iconv_convert(_kernel_swi_regs *regs)
{
	char *inbuf, *outbuf;
	size_t inbytesleft, outbytesleft;
	size_t ret;

	inbuf = (char *)regs->r[1];
	inbytesleft = (size_t)regs->r[2];
	outbuf = (char *)regs->r[3];
	outbytesleft = (size_t)regs->r[4];

	ret = iconv((iconv_t)regs->r[0], &inbuf, &inbytesleft,
			&outbuf, &outbytesleft);

	regs->r[1] = (intptr_t) inbuf;
	regs->r[2] = (intptr_t) inbytesleft;
	regs->r[3] = (intptr_t) outbuf;
	regs->r[4] = (intptr_t) outbytesleft;

	return ret;
}

int errno_to_iconv_error(int num)
{
	switch (num) {
	case ENOMEM:
		return ICONV_NOMEM;
	case E2BIG:
		return ICONV_2BIG;
	case EILSEQ:
		return ICONV_ILSEQ;
	case EINVAL:
	default:
		break;
	}

	return ICONV_INVAL;
}

