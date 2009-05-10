#ifndef _TTF2F_FT_H_
#define _TTF2F_FT_H_

#include "context.h"

struct font_metrics;
struct glyph;

void ft_init(void);
void ft_fini(void);
void *open_font(char *fname);
void close_font(void *face);
size_t count_glyphs(ttf2f_ctx *ctx);
int glnames(ttf2f_ctx *ctx);
void glmetrics(ttf2f_ctx *ctx, void (*callback)(int progress));
int glenc(ttf2f_ctx *ctx);
int fnmetrics(ttf2f_ctx *ctx);
void glpath(ttf2f_ctx *ctx, int glyphno);
void kerning(ttf2f_ctx *ctx);

#endif

