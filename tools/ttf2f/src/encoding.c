#include <stdio.h>
#include <string.h>

#include "encoding.h"
#include "fm.h"
#include "glyph.h"
#include "utils.h"

static void encoding_write_glyph(int index, struct glyph *glyph, 
		encoding_type type, FILE *fp);

/**
 * Write font encoding file
 *
 * \param savein     Location to save in
 * \param name       The font name
 * \param ctx        Conversion context
 * \param type       File format to use - 0 = full; 1 = sparse
 * \param callback   Progress callback function
 */
ttf2f_result encoding_write(const char *savein, const char *name,
		ttf2f_ctx *ctx, encoding_type type,
		void (*callback)(int progress))
{
	FILE *output;
	struct glyph *g;
	size_t i;
	char out[1024];

	snprintf(out, 1024, "%s/Encoding", savein);
	if ((output = fopen(out, "w+")) == NULL)
		return TTF2F_RESULT_OPEN;

	fprintf(output, "%% %sEncoding 1.00\n", name);
	fprintf(output, "%% Encoding file for font '%s'\n\n", name);

	/* Write latin1 first */
	for (i = 0; i != N_ELEMENTS(ctx->latin1tab); i++) {
		if (ctx->latin1tab[i] == NULL) {
			if (type == ENCODING_TYPE_NORMAL) {
				fprintf(output, "/.notdef\n");
			}
		} else {
			encoding_write_glyph(i, ctx->latin1tab[i], 
					type, output);

			ctx->latin1tab[i]->done_encoding = 1;
		}
	}

	/* Then the rest, skipping the ones we've already written */
	for (i = 0; i != ctx->nglyphs; i++) {
		g = &ctx->glyphs[i];

		callback(i * 100 / ctx->nglyphs);
		ttf2f_poll(1);

		if (g->done_encoding == 0) {
			encoding_write_glyph(i + N_ELEMENTS(ctx->latin1tab), 
					g, type, output);

			g->done_encoding = 1;
		}
	}

	fclose(output);

	return TTF2F_RESULT_OK;
}

void encoding_write_glyph(int index, struct glyph *glyph, 
		encoding_type type, FILE *fp)
{
	if (type == ENCODING_TYPE_SPARSE) {
		if (glyph->name != 0) {
			/* .notdef is implicit */
			if (strcmp(glyph->name, ".notdef") != 0) {
				fprintf(fp, "%4.4X;%s;COMMENT\n", index, 
						glyph->name);
			}
		} else if (glyph->code != (unsigned int) -1) {
			fprintf(fp, "%4.4X;uni%04X;COMMENT\n", index, 
					glyph->code);
		} else {
			fprintf(fp, "# Skipping %4.4X\n", index);
		}
	} else {
		if (glyph->name != 0) {
			fprintf(fp, "/%s\n", glyph->name);
		} else if (glyph->code != (unsigned int) -1) {
			fprintf(fp, "/uni%4.4X\n", glyph->code);
		} else {
			fprintf(fp, "/.notdef\n");
		}
	}
}


