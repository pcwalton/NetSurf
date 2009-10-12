#include <stdint.h>
#include <stdio.h>

extern void __cyg_profile_enter(void *, void *);
extern void __cyg_profile_exit(void *, void *);

extern void *image__ro__base;
extern void *image__ro__limit;

static uint32_t depth;

static void print_function_name(void *fn_address)
{
	uint32_t *offaddr = ((uint32_t *) fn_address) - 1;
	uint32_t offset = *offaddr;

	if ((offset >> 24) == 0xff) {
		fprintf(stderr, "%s\n", (const char *) (offaddr - 
				((offset & ~0xff000000) / 4)));
	} else {
		fprintf(stderr, "(unknown)\n");
	}
}

void __cyg_profile_enter(void *fn_address, void *call_site)
{
	uint32_t i = depth;

	(void) call_site;

	/* Ignore if address is out of bounds */
	if (fn_address < image__ro__base || image__ro__limit < fn_address)
		return;

	while (i-- > 0)
		fputc(' ', stderr);

	fprintf(stderr, "Entering ");
	print_function_name(fn_address);

	depth++;
}

void __cyg_profile_exit(void *fn_address, void *call_site)
{
	uint32_t i = depth;

	(void) call_site;

	/* Ignore if address is out of bounds */
	if (fn_address < image__ro__base || image__ro__limit < fn_address)
		return;

	while (i-- > 0)
		fputc(' ', stderr);

	fprintf(stderr, "Leaving ");
	print_function_name(fn_address);

	depth--;
}

