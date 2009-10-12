#include <stdint.h>
#include <stdio.h>

extern uint32_t image__ro__base;
extern uint32_t image__ro__limit;

static uint32_t depth;

static void print_function_name(uint32_t fn_address)
{
	uint32_t *offaddr = ((uint32_t *) address) - 1;
	uint32_t offset = *offaddr;

	if ((offset >> 24) == 0xff) {
		fprintf(stderr, "%s\n", (const char *) (offaddr - 
				((offset & ~0xff000000) / 4)));
	} else {
		fprintf(stderr, "(unknown)\n");
	}
}

void __cyg_profile_enter(uint32_t fn_address, uint32_t call_site)
{
	uint32_t i = depth;

	/* Ignore if address is out of bounds */
	if (fn_address < image__ro__base || image__ro__limit < fn_address)
		return;

	while (i-- > 0)
		fputc(' ', stderr);

	fprintf(stderr, "Entering ");
	print_function_name(fn_address);

	depth++;
}

void __cyg_profile_exit(uint32_t fn_address, uint32_t call_site)
{
	uint32_t i = depth;

	/* Ignore if address is out of bounds */
	if (fn_address < image__ro__base || image__ro__limit < fn_address)
		return;

	while (i-- > 0)
		fputc(' ', stderr);

	fprintf(stderr, "Leaving ");
	print_function_name(fn_address);

	depth--;
}

