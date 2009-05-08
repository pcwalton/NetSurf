#ifndef _TTF2F_INTMETRICS_H_
#define _TTF2F_INTMETRICS_H_

#include "utils.h"

struct glyph;
struct font_metrics;

ttf2f_result write_intmetrics(const char *savein,
		const char *name, struct glyph *glyph_list, 
		int list_size, struct font_metrics *metrics,
		void (*callback)(int progress));

#endif
