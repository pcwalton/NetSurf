#ifndef _TTF2F_INTMETRICS_H_
#define _TTF2F_INTMETRICS_H_

#include "utils.h"

struct glyph;
struct font_metrics;

ttf2f_result intmetrics_write(const char *savein,
		const char *name, const struct glyph *glyph_list, 
		int list_size, const struct font_metrics *metrics,
		void (*callback)(int progress));

#endif
