#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glyphs.h"

struct glyph_entry {
	unsigned short code;
	char *name;
	struct glyph_entry *next;
};

struct glyph_entry glyphs[256];

void load_glyph_list(void)
{
	FILE *fp;
	char line[1024];
	char *semi, *name;
	struct glyph_entry *g, *cur;

	fp = fopen("<TTF2f$Dir>.Glyphs", "r");
	if (!fp) {
		fprintf(stderr, "Failed opening glyphs file\n");
		exit(255);
	}

	while(fgets(line, 1024, fp)) {
		/* skip comments & blank lines */
		if (line[0] == 0 || line[0] == '#')
			continue;

		/* strip cr from end */
		line[strlen(line) - 1] = 0;

		semi = strchr(line, ';');
		if (!semi)
			continue;
		*semi = 0;
		name = semi+1;
		semi = strchr(name, ';');
		if (semi)
			*semi = 0;

		g = calloc(1, sizeof(struct glyph_entry));
		if (!g) {
			fprintf(stderr, "malloc failed\n");
			exit(255);
		}

		g->code = (unsigned short)strtoul(line, NULL, 16);
		g->name = strdup(name);

//		fprintf(stderr, "%04.4X: %s\n", g->code, g->name);

		for (cur = &glyphs[g->code / 256];
				cur->next && cur->code < g->code;
				cur = cur->next)
			;

		if (cur->code == g->code) {
			free(g->name);
			free(g);
			continue;
		}

		if (cur->next)
			g->next = cur->next;
		cur->next = g;
	}

	fclose(fp);
}

char *glyph_name(unsigned short code)
{
	struct glyph_entry *g;

	for (g = &glyphs[code / 256]; g; g = g->next)
		if (g->code == code)
			break;

	if (!g)
		return NULL;

	return g->name;
}

void destroy_glyphs(void)
{
	int i;
	struct glyph_entry *a, *b;

	for (i = 0; i != 256; i++) {
		for (a = (&glyphs[i])->next; a; a = b) {
			b = a->next;
			free(a->name);
			free(a);
		}
	}
}
