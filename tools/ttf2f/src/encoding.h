#ifndef _TTF2F_ENCODING_H_
#define _TTF2F_ENCODING_H_

#include "utils.h"

struct glyph;

ttf2f_result write_encoding(const char *savein, const char *name,
		struct glyph *glyph_list, int list_size, int type,
		void (*callback)(int progress));

#endif

