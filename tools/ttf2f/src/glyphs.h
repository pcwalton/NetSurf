#ifndef _TTF2F_GLYPHS_H_
#define _TTF2F_GLYPHS_H_

#include "utils.h"

ttf2f_result glyph_load_list(void);
void glyph_destroy_list(void);
const char *glyph_name(unsigned short code);

#endif
