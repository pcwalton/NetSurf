#ifndef _TTF2F_INTMETRICS_H_
#define _TTF2F_INTMETRICS_H_

#include "context.h"
#include "utils.h"

struct glyph;
struct font_metrics;

ttf2f_result intmetrics_write(const char *savein,
		const char *name, ttf2f_ctx *ctx,
		void (*callback)(int progress));

#endif
