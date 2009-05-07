#ifndef _TTF2F_INTMETRICS_H_
#define _TTF2F_INTMETRICS_H_

struct glyph;
struct font_metrics;

void write_intmetrics(const char *savein, const char *name,
		struct glyph *glyph_list, int list_size,
		struct font_metrics *metrics,
		void (*callback)(int progress));

#endif
