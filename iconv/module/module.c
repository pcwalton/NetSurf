/* Iconv module interface */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iconv-internal/iconv.h>

#include "errors.h"
#include "header.h"
#include "module.h"

static _kernel_oserror ErrorGeneric = { 0x0, "" };

static size_t iconv_convert(_kernel_swi_regs *r);
static _kernel_oserror *do_iconv(int argc, const char *args);
static int errno_to_iconv_error(int num);
static const char *get_aliases_path(void);

/* Module initialisation */
_kernel_oserror *mod_init(const char *tail, int podule_base, void *pw)
{
	char *ucpath;
	const char *aliases;

	UNUSED(tail);
	UNUSED(podule_base);
	UNUSED(pw);

	/* ensure the !Unicode resource exists */
#ifdef __riscos__
	ucpath = getenv("Unicode$Path");
#else
	ucpath = getenv("UNICODE_DIR");
#endif

	if (ucpath == NULL) {
		strncpy(ErrorGeneric.errmess, "!Unicode resource not found.",
				252);
		return &ErrorGeneric;
	}

	aliases = get_aliases_path();

	if (iconv_initialise(aliases) == false) {
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
	case CMD_Iconv:
		return do_iconv(argc, arg_string);
		break;
	case CMD_ReadAliases:
	{
		const char *aliases = get_aliases_path();

		if (aliases == NULL)
			return NULL;

		free_alias_data();
		if (!create_alias_data(aliases)) {
			strcpy(ErrorGeneric.errmess,
					"Failed reading Aliases file.");
			return &ErrorGeneric;
		}
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

_kernel_oserror *do_iconv(int argc, const char *args)
{
	char from[64] = "", to[64] = "";
	char *f, *t;
	bool list = false, verbose = false, stop_on_invalid = true;
	char out[4096] = "";
	char *o;
	const char *p = args;
	FILE *ofp;
	iconv_t cd;

	/* Parse options */
	while (argc > 0 && *p == '-') {
		if (*(p+1) < ' ')
			break;

		switch (*(p+1)) {
		case 'f':
			f = from;
			p += 2;
			if (*p == ' ') {
				argc--;
				while (*p == ' ')
					p++;
			}
			while (*p > ' ') {
				if (f - from < (int) sizeof(from) - 1)
					*f++ = *p;
				p++;
			}
			*f = '\0';
			while (*p == ' ')
				p++;
			argc--;
			break;
		case 't':
			t = to;
			p += 2;
			if (*p == ' ') {
				argc--;
				while (*p == ' ')
					p++;
			}
			while (*p > ' ') {
				if (t - to < (int) sizeof(to) - 1)
					*t++ = *p;
				p++;
			}
			*t = '\0';
			while (*p == ' ')
				p++;
			argc--;
			break;
		case 'l':
			list = true;
			p += 2;
			argc--;
			break;
		case 'o':
			o = out;
			p += 2;
			if (*p == ' ') {
				argc--;
				while (*p == ' ')
					p++;
			}
			while (*p > ' ') {
				if (o - out < (int) sizeof(out) - 1)
					*o++ = *p;
				p++;
			}
			*o = '\0';
			while (*p == ' ')
				p++;
			argc--;
			break;
		case 'v':
			verbose = true;
			p += 2;
			argc--;
			break;
		case 'c':
			stop_on_invalid = false;
			p += 2;
			argc--;
			break;
		case 's':
		default:
			snprintf(ErrorGeneric.errmess, 
				sizeof(ErrorGeneric.errmess),
				"Iconv: invalid option -- %c", *(p+1));
			return &ErrorGeneric;
		}
	}

	if (list) {
		printf("Here's a list of all the encoding names that Iconv "
		       "knows about:.\n\n");
		dump_alias_data();
		return NULL;
	}

	/** \todo better defaults */
	if (from[0] == '\0')
		strcpy(from, "ISO-8859-1");

	if (to[0] == '\0')
		strcpy(to, "ISO-8859-1");

	cd = iconv_open(to, from);
	if (cd == (iconv_t) -1) {
		snprintf(ErrorGeneric.errmess, sizeof(ErrorGeneric.errmess),
			"Cannot convert from %s to %s", from, to);
		return &ErrorGeneric;
	}

	if (out[0] == '\0')
		ofp = stdout;
	else
		ofp = fopen(out, "w");
	if (ofp == NULL) {
		iconv_close(cd);
		strcpy(ErrorGeneric.errmess, "Unable to open output file.");
		return &ErrorGeneric;
	}

	while (argc-- > 0) {
		/* Remaining parameters are input files */
		while (*p == ' ')
			p++;

		args = p;

		while (*p > ' ')
			p++;

		char fname[p - args + 1];

		memcpy(fname, args, p - args);
		fname[p - args] = '\0';

		FILE *inf = fopen(fname, "r");
		if (inf == NULL) {
			iconv_close(cd);
			fclose(ofp);
			snprintf(ErrorGeneric.errmess, 
					sizeof(ErrorGeneric.errmess),
					"Failed opening input file '%s'",
					fname);
			return &ErrorGeneric;
		}

		fseek(inf, 0, SEEK_END);
		size_t input_length = ftell(inf);
		fseek(inf, 0, SEEK_SET);

		/* Perform conversion, using fixed size buffers. */
		char input[1024];
		char output[4096];
		size_t leftover = 0;

#ifndef min
#define min(x,y) (x) < (y) ? (x) : (y)
#endif

		while (input_length > 0) {
			char *in = input;
			char *out = output;
			size_t inbytes = min(sizeof(input), 
					input_length + leftover);
			size_t inlen = inbytes;
			size_t outlen = sizeof(output);

			/* Fill input buffer */
			fread(input + leftover, 1, inlen - leftover, inf);

			/* Convert text */
			size_t read = iconv(cd, &in, &inlen, &out, &outlen);
			if (read == (size_t) -1) {
				switch (errno) {
				case EILSEQ:
					if (verbose) {
						fprintf(stderr, 
							"Illegal multibyte "
							"character sequence: "
							"'%10s'\n", in);
					}
					if (stop_on_invalid) {
						iconv_close(cd);
						fclose(ofp);
						fclose(inf);
						return NULL;
					}
					break;
				default:
					break;
				}
			}

			/* Write output buffer */
			fwrite(output, 1, sizeof(output) - outlen, ofp);

			/* Save leftover for next time */
			leftover = inlen;
			memmove(input, in, leftover);

			input_length -= (inbytes - leftover);
		}

		/* Flush through any remaining shift sequences */
		char *out = output;
		size_t outlen = sizeof(output);

		iconv(cd, NULL, NULL, &out, &outlen);

		fwrite(output, 1, sizeof(output) - outlen, ofp);

		/* Reset cd ready for next file */
		iconv(cd, NULL, NULL, NULL, NULL);

		fclose(inf);
	}

	if (ofp != stdout)
		fclose(ofp);

	iconv_close(cd);

	return NULL;
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

const char *get_aliases_path(void)
{
	const char *ucpath;
	int alen;
	static char aliases[4096];

#ifdef __riscos__
#define ALIASES_FILE "Files.Aliases"
#else
#define ALIASES_FILE "Files/Aliases"
#endif

	/* Get !Unicode resource path */
#ifdef __riscos__
	ucpath = "Unicode:";
#else
	ucpath = getenv("UNICODE_DIR");
#endif

	if (ucpath == NULL)
		return NULL;

	strncpy(aliases, ucpath, sizeof(aliases));
	alen = strlen(ucpath);
#ifndef __riscos__
	if (aliases[alen - 1] != '/') {
		strncat(aliases, "/", sizeof(aliases) - alen - 1);
		alen += 1;
	}
#endif
	strncat(aliases, ALIASES_FILE, sizeof(aliases) - alen - 1);
	aliases[sizeof(aliases) - 1] = '\0';

	return aliases;
}

