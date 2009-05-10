#ifndef _TTF2F_ENCODING_H_
#define _TTF2F_ENCODING_H_

#include "context.h"
#include "utils.h"

typedef enum encoding_type {
	ENCODING_TYPE_NORMAL,
	ENCODING_TYPE_SPARSE
} encoding_type;

struct glyph;

ttf2f_result encoding_write(const char *savein, const char *name,
		ttf2f_ctx *ctx, encoding_type type,
		void (*callback)(int progress));

#endif

