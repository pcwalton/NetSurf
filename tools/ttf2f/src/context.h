#ifndef ttf2f_context_h_
#define ttf2f_context_h_

#include "fm.h"
#include "glyph.h"

typedef struct ttf2f_ctx ttf2f_ctx;

struct ttf2f_ctx {
	void *face;

	struct font_metrics *metrics;

	size_t nglyphs;
	struct glyph *glyphs;

	struct glyph *latin1tab[256 - 32]; /* Not chunk zero */
};

#endif
