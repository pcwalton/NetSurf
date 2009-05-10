#include <stdio.h>
#include <string.h>

#include "encoding.h"
#include "fm.h"
#include "glyph.h"
#include "utils.h"

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

	snprintf(out, 1024, "%s" DIR_SEP "Encoding", savein);
	if ((output = fopen(out, "w+")) == NULL)
		return TTF2F_RESULT_OPEN;

	fprintf(output, "%% %sEncoding 1.00\n", name);
	fprintf(output, "%% Encoding file for font '%s'\n\n", name);

	if (type == ENCODING_TYPE_NORMAL) {
		for (i = 0; i != 32; i++) {
			fprintf(output, "/.notdef\n");
		}
	}

	for (i = 0; i != ctx->nglyphs; i++) {
		g = &ctx->glyphs[i];

		callback(i * 100 / ctx->nglyphs);
		ttf2f_poll(1);

		if (type == ENCODING_TYPE_SPARSE) {
			if (g->name != 0) {
				/* .notdef is implicit */
				if (strcmp(g->name, ".notdef") == 0)
					continue;
				fprintf(output, "%4.4zX;%s;COMMENT\n", i+32,
							g->name);
			} else if (g->code != (unsigned int) -1) {
				fprintf(output, "%4.4zX;uni%04X;COMMENT\n",
							i+32, g->code);
			} else {
				fprintf(output, "# Skipping %4.4zX\n", i+32);
			}
		} else {
			if (g->name != 0) {
				fprintf(output, "/%s\n", g->name);
			} else if (g->code != (unsigned int) -1) {
				fprintf(output, "/uni%4.4X\n", g->code);
			} else {
				fprintf(output, "/.notdef\n");
			}
		}
	}

	fclose(output);

	return TTF2F_RESULT_OK;
}

