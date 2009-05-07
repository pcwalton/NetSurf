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

static short mapsize;

static char *character_map = 0;

/* we don't use these */
// static short *x0;
// static short *y0;
// static short *x1;
// static short *y1;

static short *xwidthtab = 0;
// static short *ywidthtab;

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
void write_intmetrics(const char *savein, const char *name,
		struct glyph *glyph_list, int list_size,
		struct font_metrics *metrics,
		void (*callback)(int progress))
{
	struct intmetrics_header header;
	int charmap_size = 0;
	unsigned int xwidthtab_size = 0;
	int i;
	struct glyph *g;
	char out[1024];
	FILE *output;

	UNUSED(metrics);

	/* allow for chunk 0 */
	xwidthtab = calloc(33, sizeof(short));
	if (!xwidthtab) {
		fprintf(stderr, "malloc failed\n");
		return;
	}
	xwidthtab_size = 32;

	/* create xwidthtab - char code is now the index */
	for (i = 0; i != list_size; i++) {
		g = &glyph_list[i];

		callback((i * 100) / list_size);
		ttf2f_poll(1);

		xwidthtab_size++;
		xwidthtab[i+32] = g->width;
		xwidthtab = realloc(xwidthtab,
				(xwidthtab_size+1) * sizeof(short));
		if (!xwidthtab) {
			fprintf(stderr, "malloc failed\n");
			return;
		}
	}

	/* fill in header */
	snprintf(header.name, 40, "%s", name);
	memset(header.name + (strlen(name) >= 40 ? 39 : strlen(name)), 0xD,
		40 - (strlen(name) >= 40 ? 39 : strlen(name)));
	header.a = header.b = 16;
	header.version = 0x2;
	header.flags = 0x25;
	header.nhi = xwidthtab_size / 256;
	header.nlo = xwidthtab_size % 256;

	mapsize = charmap_size;

	snprintf(out, 1024, "%s.IntMetrics", savein);
	output = fopen(out, "wb+");
	fwrite((void*)&header, sizeof(struct intmetrics_header), 1, output);
	fputc(mapsize & 0xFF, output);
	fputc((mapsize & 0xFF00) >> 8, output);
	fwrite(character_map, sizeof(char), charmap_size, output);
	fwrite(xwidthtab, sizeof(short), xwidthtab_size, output);
	fclose(output);

#ifdef __riscos__
	/* set type */
	_swix(OS_File, _INR(0,2), 18, out, 0xFF6);
#endif

	if (character_map)
		free(character_map);
	free(xwidthtab);
}

