#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __riscos__
#include "swis.h"
#endif

#include "fm.h"
#include "glyph.h"
#include "intmetrics.h"
#include "utils.h"

struct intmetrics_header {
	char  name[40];
	int   a;
	int   b;
	char  nlo;
	char  version;
	char  flags;
	char  nhi;
};

struct extra_data_offsets {
	short misc;
	short kern;
	short res1;
	short res2;
};

struct misc_area {
	short x0;
	short y0;
	short x1;
	short y1;
	short default_x_offset;
	short default_y_offset;
	short italic_h_offset;
	char  underline_position;
	char  underline_thickness;
	short cap_height;
	short xheight;
	short descender;
	short ascender;
	int reserved;
};

struct kern_pair_8 {
	char  left;
	char  right;
	short x_kern;
	short y_kern;
	char  list_end;
	char  area_end;
};

struct kern_pair_16 {
	short left;
	short right;
	short x_kern;
	short y_kern;
	short list_end;
	short area_end;
};

/**
 * Write font metrics to file
 *
 * \param savein     Location to save in
 * \param name       Font name
 * \param context    Conversion context
 */
ttf2f_result intmetrics_write(const char *savein, const char *name, 
		ttf2f_ctx *ctx, void (*callback)(int progress))
{
	struct intmetrics_header header;
	short *xwidthtab = NULL;
	unsigned int xwidthtab_size = 0;
	int xwidthtab_idx = N_ELEMENTS(ctx->latin1tab);
	short mapsize;
	size_t i, name_len;
	struct glyph *g;
	char out[1024];
	FILE *output;

	/* Total number of slots is the number of glyphs plus any spare
	 * required for the latin1 table */
	xwidthtab_size = ctx->nglyphs + 
			N_ELEMENTS(ctx->latin1tab) - ctx->nlatin1;

	xwidthtab = calloc(xwidthtab_size, sizeof(short));
	if (xwidthtab == NULL)
		return TTF2F_RESULT_NOMEM;

	/* fill latin1 subset first */
	for (i = 0; i != N_ELEMENTS(ctx->latin1tab); i++) {
		g = ctx->latin1tab[i];

		xwidthtab[i] = g != NULL ? g->width : 0;

		if (g != NULL)
			g->done_intmetrics = 1;
	}

	/* then the rest, skipping those we've already written */
	for (i = 0; i != ctx->nglyphs; i++) {
		g = &ctx->glyphs[i];

		callback((i * 100) / ctx->nglyphs);
		ttf2f_poll(1);

		if (g->done_intmetrics == 0) {
			xwidthtab[xwidthtab_idx++] = g->width;
			g->done_intmetrics = 1;
		}
	}

	/* fill in header */
	name_len = min(39, strlen(name));
	snprintf(header.name, 40, "%s", name);
	memset(header.name + name_len, 0xD, 40 - name_len);
	header.a = header.b = 16;
	header.version = 0x2;
	/* Character map size before map | No y-offset data | No bbox data */
	header.flags = 0x25; 
	header.nhi = xwidthtab_size / 256;
	header.nlo = xwidthtab_size % 256;

	/* No character map */
	mapsize = 0;

	snprintf(out, 1024, "%s/IntMetrics", savein);
	if ((output = fopen(out, "wb")) == NULL) {
		free(xwidthtab);
		return TTF2F_RESULT_OPEN;
	}

	if (fwrite(&header, sizeof(struct intmetrics_header), 1, output) != 1)
		goto error_write;

	if (fputc(mapsize & 0xFF, output) == EOF) goto error_write;
	if (fputc((mapsize & 0xFF00) >> 8, output) == EOF) goto error_write;

	if (fwrite(xwidthtab, sizeof(short), xwidthtab_size, output)
		!= xwidthtab_size) goto error_write;

	/** \todo miscellaneous data */

	fclose(output);

#ifdef __riscos__
	/* set type */
	_swix(OS_File, _INR(0,2), 18, out, 0xFF6);
#endif

	free(xwidthtab);
	
	return TTF2F_RESULT_OK;

error_write:
	free(xwidthtab);
	fclose(output);

	return TTF2F_RESULT_WRITE;
}

