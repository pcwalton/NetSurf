#ifndef _TTF2F_FT_H_
#define _TTF2F_FT_H_

struct font_metrics;
struct glyph;

void ft_init(void);
void ft_fini(void);
void *open_font(char *fname);
void close_font(void *face);
size_t count_glyphs(void *face);
int glnames(void *face, struct glyph *glyph_list);
void glmetrics(void *face, struct glyph *glyph_list, 
		void (*callback)(int progress));
int glenc(void *face, struct glyph *glyph_list);
int fnmetrics(void *face, struct font_metrics *fm);
void glpath(void *face, int glyphno, struct glyph *glyph_list);
void kerning(void *face, struct glyph *glyph_list);

#endif

