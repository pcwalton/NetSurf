#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>

#include "encoding.h"
#include "fm.h"
#include "ft.h"
#include "glyph.h"
#include "glyphs.h"
#include "intmetrics.h"
#include "outlines.h"
#include "utils.h"

static void progress(int value)
{
	UNUSED(value);
}

void ttf2f_poll(int active)
{
	UNUSED(active);
}

int main(int argc, char **argv)
{
	int fail;
	int nglyphs;
	struct glyph *glist;
	struct font_metrics *metrics;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s <input.ttf> <output>\n", argv[0]);
		return 1;
	}

	ft_init();
	load_glyph_list();

	fail = open_font(argv[1]);
	if (fail) {
		fprintf(stderr, "ERROR: Failed opening font %s\n", argv[1]);
		return 1;
	}

	nglyphs = count_glyphs();

	glist = calloc(nglyphs, sizeof(struct glyph));
	if (glist == NULL) {
		fprintf(stderr, "ERROR: insufficient memory for glyphs\n");
		return 1;
	}

	for (int i = 0; i != nglyphs; i++) {
		struct glyph *g = &glist[i];

		g->code = -1;
	}

	metrics = calloc(1, sizeof(struct font_metrics));
	if (metrics == NULL) {
		fprintf(stderr, "ERROR: insufficient memory for font metrics\n");
		return 1;
	}

	fail = fnmetrics(metrics);
	if (fail) {
		fprintf(stderr, "ERROR: failed reading font metrics\n");
		return 1;
	}

	fail = glenc(glist);
	if (fail) {
		fprintf(stderr, "ERROR: failed reading glyph encoding\n");
		return 1;
	}

	fail = glnames(glist);
	if (fail) {
		fprintf(stderr, "ERROR: failed reading glyph names\n");
		return 1;
	}

	glmetrics(glist, progress);

	mkdir(argv[2], 0755);

	write_intmetrics(argv[2], argv[2], glist, nglyphs, metrics, progress);

	write_outlines(argv[2], argv[2], glist, nglyphs, metrics, progress);

	write_encoding(argv[2], argv[2], glist, nglyphs, 0, progress);

	free(metrics);
	free(glist);

	close_font();

	ft_fini();
	destroy_glyphs();

	return 0;
}

