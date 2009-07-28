#include <stdio.h>

struct profile {
	int fcc;
	long long exclusive_time;
	long long inclusive_time;
};

extern void profile_save(struct profile **table, int size, void *ro_base);

/**
 * Save the profiling data as a CSV file
 *
 * \param table    Pointer to table of pointers to profile data
 * \param size     Number of table entries
 * \param ro_base  Base address of code in loaded executable
 */
extern void profile_save(struct profile **table, int size, void *ro_base)
{
	FILE *fp;
	struct profile **p;
	const char *fname;
	unsigned int offset;

	fp = fopen("$.profile_save/csv", "w");
	if (!fp)
		return;

	for (p = table; p < table+size; p++) {
		if (!p || !*p)
			continue;

		/* For code with embedded function names, the word before
		 * the function entry point contains the length of the
		 * function name ORred together with 0xFF000000. Therefore,
		 * we read the word before the function entry point here.
		 */
		offset = *((int *)ro_base + (p - table) - 1);

		fname = ((offset >> 24) == 0xff)
			?
				/* embedded function name => read it */
				(const char *)
					((int *)ro_base + (p - table - 1) -
					((offset & ~0xff000000) / 4))
			:
				/* no function name */
				"";

		fprintf(fp, "%s,%d,%lld,%lld\n",
			/*((p - table) * 4) + (int)ro_base,*/
			fname,			/* function name */
			(*p)->fcc,		/* function call count */
			(*p)->exclusive_time,	/* exclusive execution time */
			(*p)->inclusive_time);	/* inclusive execution time */
	}

	fclose(fp);
}

