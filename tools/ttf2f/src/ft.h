#ifndef _TTF2F_FT_H_
#define _TTF2F_FT_H_

struct font_metrics;
struct glyph;

void ft_init(void);
void ft_fini(void);
int open_font(char *fname);
void close_font(void);
int count_glyphs(void);
int glnames(struct glyph *glyph_list);
void glmetrics(struct glyph *glyph_list, void (*callback)(int progress));
int glenc(struct glyph *glyph_list);
int fnmetrics(struct font_metrics *fm);
void glpath(int glyphno, struct glyph *glyph_list);
void kerning(struct glyph *glyph_list);

#endif

