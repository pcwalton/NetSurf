#ifndef _TTF2F_ENCODING_H_
#define _TTF2F_ENCODING_H_

#include "utils.h"

typedef enum encoding_type {
	ENCODING_TYPE_NORMAL,
	ENCODING_TYPE_SPARSE
} encoding_type;

struct glyph;

ttf2f_result encoding_write(const char *savein, const char *name,
		struct glyph *glyph_list, int list_size, encoding_type type,
		void (*callback)(int progress));

#endif

