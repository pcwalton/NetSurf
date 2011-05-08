#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>

#ifdef __riscos__
#include <unixlib/local.h>
#endif

#include "context.h"
#include "encoding.h"
#include "fm.h"
#include "ft.h"
#include "glyph.h"
#include "glyphs.h"
#include "intmetrics.h"
#include "outlines.h"
#include "utils.h"

#ifdef __riscos__
int __riscosify_control = __RISCOSIFY_NO_PROCESS;
#endif

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
	ttf2f_ctx ctx;
	int fail;
	ttf2f_result err = TTF2F_RESULT_OK;
	char *dot, *path;
	char full_path[PATH_MAX]; /* Lazy */

	if (argc != 4) {
		fprintf(stderr, 
			"Usage: %s <input.ttf> <destdir> <font name>\n"
			"<destdir> must be in the platform's path format\n"
			"<font name> must be in the form Trinity.Bold\n", 
			argv[0]);
		return 1;
	}

	memset(&ctx, 0, sizeof(ctx));

	ft_init();

	if ((err = glyph_load_list()) != TTF2F_RESULT_OK)
		goto error_out;

	ctx.face = open_font(argv[1]);
	if (ctx.face == NULL) {
		fprintf(stderr, "ERROR: Failed opening font %s\n", argv[1]);
		return 1;
	}

	ctx.nglyphs = count_glyphs(&ctx);

	ctx.glyphs = calloc(ctx.nglyphs, sizeof(struct glyph));
	if (ctx.glyphs == NULL) {
		fprintf(stderr, "ERROR: insufficient memory for glyphs\n");
		return 1;
	}

	for (size_t i = 0; i != ctx.nglyphs; i++) {
		struct glyph *g = &ctx.glyphs[i];

		g->code = -1;
	}

	ctx.metrics = calloc(1, sizeof(struct font_metrics));
	if (ctx.metrics == NULL) {
		fprintf(stderr, 
			"ERROR: insufficient memory for font metrics\n");
		return 1;
	}

	fail = fnmetrics(&ctx);
	if (fail) {
		fprintf(stderr, "ERROR: failed reading font metrics\n");
		return 1;
	}

	fail = glenc(&ctx);
	if (fail) {
		fprintf(stderr, "ERROR: failed reading glyph encoding\n");
		return 1;
	}

	fail = glnames(&ctx);
	if (fail) {
		fprintf(stderr, "ERROR: failed reading glyph names\n");
		return 1;
	}

	glmetrics(&ctx, progress);

	mkdir(argv[2], 0755);

	strncpy(full_path, argv[2], sizeof(full_path));
	path = full_path + strlen(full_path);

	if (*(path - 1) != *DIR_SEP)
		*(path++) = *DIR_SEP;

	for (dot = argv[3]; *dot != '\0'; dot++) {
		if (*dot == '.') {
			*path = '\0';
			mkdir(full_path, 0755);
			*(path++) = *DIR_SEP;
		} else {
			*(path++) = *dot;
		}
	}
	*path = '\0';
	mkdir(full_path, 0755);

	if ((err = intmetrics_write(full_path, argv[3], &ctx, progress)) != 
			TTF2F_RESULT_OK) goto error_out;

	if ((err = outlines_write(full_path, argv[3], &ctx, progress)) != 
			TTF2F_RESULT_OK) goto error_out;

	if ((err = encoding_write(full_path, argv[3], &ctx, 
			ENCODING_TYPE_NORMAL, progress)) != 
			TTF2F_RESULT_OK) goto error_out;

error_out:
	if (err != TTF2F_RESULT_OK) {
		switch (err) {
		case TTF2F_RESULT_NOMEM:
			fprintf(stderr, "ERROR: failed to allocate memory\n");
			break;
		case TTF2F_RESULT_OPEN:
			fprintf(stderr, "ERROR: failed to open output file\n");
			break;
		case TTF2F_RESULT_WRITE:
			fprintf(stderr, "ERROR: failed to write to file\n");
			break;

		case TTF2F_RESULT_OK:
			/* keeps gcc quiet, even though this will never occur.
			 * we avoid default: so we get a warning is we add more
			 */
			break;
		}
	}

	if (ctx.metrics != NULL) {
		free(ctx.metrics->name_copyright);
		free(ctx.metrics->name_full);
		free(ctx.metrics->name_version);
		free(ctx.metrics->name_ps);
		free(ctx.metrics);
	}

	free(ctx.glyphs);

	if (ctx.face != NULL)
		close_font(ctx.face);

	ft_fini();
	glyph_destroy_list();

	exit(err == TTF2F_RESULT_OK ? EXIT_SUCCESS : EXIT_FAILURE);
}

