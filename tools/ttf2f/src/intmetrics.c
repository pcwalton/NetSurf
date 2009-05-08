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
 * \param glyph_list List of all glyphs in font
 * \param list_size  Size of glyph list
 * \param metrics    Global font metrics
 */
ttf2f_result intmetrics_write(const char *savein, const char *name, 
		const struct glyph *glyph_list, int list_size,
		const struct font_metrics *metrics, 
		void (*callback)(int progress))
{
	struct intmetrics_header header;
	short *xwidthtab = NULL;
	unsigned int xwidthtab_size = 0;
	short mapsize;
	int i, name_len;
	const struct glyph *g;
	char out[1024];
	FILE *output;

	UNUSED(metrics);

	/* allow for chunk 0 */
	xwidthtab = calloc(33, sizeof(short));
	if (xwidthtab == NULL)
		return TTF2F_RESULT_NOMEM;

	xwidthtab_size = 32;

	/* create xwidthtab - char code is now the index */
	for (i = 0; i != list_size; i++) {
		short *temp;

		g = &glyph_list[i];

		callback((i * 100) / list_size);
		ttf2f_poll(1);

		xwidthtab_size++;
		/* +32 to skip chunk 0 */
		xwidthtab[i + 32] = g->width;
		temp = realloc(xwidthtab, (xwidthtab_size + 1) * sizeof(short));
		if (temp == NULL) {
			free(xwidthtab);
			return TTF2F_RESULT_NOMEM;
		}
		xwidthtab = temp;
	}

	/* fill in header */
	name_len = max(39, strlen(name));
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

	snprintf(out, 1024, "%s" DIR_SEP "IntMetrics", savein);
	if ((output = fopen(out, "wb+")) == NULL) {
		free(xwidthtab);
		return TTF2F_RESULT_OPEN;
	}

	if (fwrite((void*)&header, sizeof(struct intmetrics_header),
		   1, output) != 1) goto error_write;

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

