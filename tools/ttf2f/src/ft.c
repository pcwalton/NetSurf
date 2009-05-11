/*
 * The font parser using the FreeType library version 2.
 *
 * based in part upon the ft.c source file in TTF2PT1
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ft2build.h"
#include FT_FREETYPE_H
#include "freetype/freetype.h"
#include "freetype/ftglyph.h"
#include "freetype/ftsnames.h"
#include "freetype/ttnameid.h"
#include "freetype/ftoutln.h"
#include "freetype/tttables.h"

#include "ft.h"
#include "fm.h"
#include "glyph.h"
#include "glyphs.h"
#include "utils.h"

static FT_Library library;

/**
 * Acorn Latin 1 characters
 */
static unsigned int acornLatin1[32] = {
  0x20ac, 0x0174, 0x0175, 0xffff, 0xffff, 0x0176, 0x0177, 0xffff, 
  0xffff, 0xffff, 0xffff, 0xffff, 0x2026, 0x2122, 0x2030, 0x2022, 
  0x2018, 0x2019, 0x2039, 0x203a, 0x201c, 0x201d, 0x201e, 0x2013, 
  0x2014, 0x2212, 0x0152, 0x0153, 0x2020, 0x2021, 0xfb01, 0xfb02
};

void ft_init(void)
{
	if (FT_Init_FreeType(&library)) {
		fprintf(stderr, "** FreeType initialization failed\n");
		exit(1);
	}
}

void ft_fini(void)
{
	if (FT_Done_FreeType(library)) {
		fprintf(stderr, "Errors when stopping FreeType, ignored\n");
	}
}

/*
 * Open font and prepare to return information to the main driver.
 * May print error and warning messages.
 */

void *open_font(char *fname)
{
	FT_Face face;
	FT_Error error;

	if ((error = FT_New_Face(library, fname, 0, &face)) != 0) {
		if (error == FT_Err_Unknown_File_Format) {
			fprintf(stderr, 
				"**** %s has format unknown to FreeType\n", 
				fname);
		} else
			fprintf(stderr, "**** Cannot access %s ****\n", fname);
		return NULL;
	}

	return face;
}

/*
 * Close font.
 * Exit on error.
 */

void close_font(void *face)
{
	if (FT_Done_Face((FT_Face) face)) {
		fprintf(stderr, "Errors when closing the font file, ignored\n");
	}
}

/*
 * Get the number of glyphs in font.
 */

size_t count_glyphs(ttf2f_ctx *ctx)
{
	FT_Face f = (FT_Face) ctx->face;

	return (size_t) f->num_glyphs;
}

/*
 * Get the names of the glyphs and populate the latin1 table
 * Returns 0 if the names were assigned, non-zero on error
 */

int glnames(ttf2f_ctx *ctx)
{
	FT_Face f = (FT_Face) ctx->face;
	int i;

	for (i = 0; i != f->num_glyphs; i++) {
		unsigned int code = ctx->glyphs[i].code;

		ttf2f_poll(1);
		ctx->glyphs[i].name = glyph_name(code);

		/* Insert into latin1 table, if appropriate */
		if (0x0020 <= code && code < 0x007f)
			ctx->latin1tab[code] = &ctx->glyphs[i];
		else if (0x00a0 <= code && code <= 0x00ff)
			ctx->latin1tab[code] = &ctx->glyphs[i];
		else {
			int j;

			for (j = 0; j < 32; j++) {
				/* Skip unmapped chars */
				if (acornLatin1[j] == 0xffff)
					continue;

				if (code == acornLatin1[j])
					break;
			}

			if (j != 32) {
				ctx->latin1tab[j + 0x80] = &ctx->glyphs[i];
			}
		}
	}

	for (i = 0; i != N_ELEMENTS(ctx->latin1tab); i++) {
		if (ctx->latin1tab[i] != NULL)
			ctx->nlatin1++;
	}

	return 0;
}

/*
 * Get the metrics of the glyphs.
 */

void glmetrics(ttf2f_ctx *ctx, void (*callback)(int progress))
{
	FT_Face f = (FT_Face) ctx->face;
	struct glyph *g;
	int i;
	FT_Glyph_Metrics *met;
	FT_BBox bbox;
	FT_Glyph gly;

	for (i = 0; i < f->num_glyphs; i++) {
		g = &ctx->glyphs[i];

		callback(i * 100 / f->num_glyphs);
		ttf2f_poll(1);

		if (FT_Load_Glyph(f, i, FT_LOAD_NO_BITMAP|FT_LOAD_NO_SCALE)) {
			fprintf(stderr, "Can't load glyph %s, skipped\n", 
					g->name);
			continue;
		}

		met = &f->glyph->metrics;

		if (FT_HAS_HORIZONTAL(f)) {
			g->width = convert_units(met->horiAdvance,
							f->units_per_EM);
			g->lsb = convert_units(met->horiBearingX,
							f->units_per_EM);
		} else {
			fprintf(stderr, "Glyph %s has no horizontal metrics\n",
					g->name);
			g->width = convert_units(met->width,
							f->units_per_EM);
			g->lsb = 0;
		}

		if (FT_Get_Glyph(f->glyph, &gly)) {
			fprintf(stderr, 
				"Can't access glyph %s bbox, skipped\n", 
				g->name);
			continue;
		}

		FT_Glyph_Get_CBox(gly, ft_glyph_bbox_unscaled, &bbox);
		g->xMin = convert_units(bbox.xMin, f->units_per_EM);
		g->yMin = convert_units(bbox.yMin, f->units_per_EM);
		g->xMax = convert_units(bbox.xMax, f->units_per_EM);
		g->yMax = convert_units(bbox.yMax, f->units_per_EM);

		g->ttf_pathlen = f->glyph->outline.n_points;

		FT_Done_Glyph(gly);
	}
}

/*
 * Map charcodes to glyph ids using the unicode encoding
 */

int glenc(ttf2f_ctx *ctx)
{
	FT_Face f = (FT_Face) ctx->face;
	unsigned charcode, glyphid;

	if (!f->charmaps || FT_Select_Charmap(f, FT_ENCODING_UNICODE)) {
		fprintf(stderr, "**** Cannot set charmap in FreeType ****\n");
		return 1;
	}

	charcode = FT_Get_First_Char(f, &glyphid);
	while (glyphid != 0) {
		ttf2f_poll(1);
		ctx->glyphs[glyphid].code = charcode;
		charcode = FT_Get_Next_Char(f, charcode, &glyphid);
	}

	return 0;
}

/*
 * Get the font metrics
 */
int fnmetrics(ttf2f_ctx *ctx)
{
	FT_Face f = (FT_Face) ctx->face;
	struct font_metrics *fm = ctx->metrics;
	const char *str;
	const char *fieldstocheck[3];
	FT_SfntName sn;
	TT_Postscript *post;
	int i, j, len;

	fm->underline_position = convert_units(f->underline_position,
						f->units_per_EM);
	fm->underline_thickness = convert_units(f->underline_thickness,
						f->units_per_EM);
	fm->is_fixed_pitch = FT_IS_FIXED_WIDTH(f);

	fm->ascender = convert_units(f->ascender, f->units_per_EM);
	fm->descender = convert_units(f->descender, f->units_per_EM);

	fm->units_per_em =  f->units_per_EM;

	fm->bbox[0] = convert_units(f->bbox.xMin, f->units_per_EM);
	fm->bbox[1] = convert_units(f->bbox.yMin, f->units_per_EM);
	fm->bbox[2] = convert_units(f->bbox.xMax, f->units_per_EM);
	fm->bbox[3] = convert_units(f->bbox.yMax, f->units_per_EM);

	if ((post = (TT_Postscript*) FT_Get_Sfnt_Table(f, 
			ft_sfnt_post)) != NULL) {
		fm->italic_angle = post->italicAngle;
	} else {
		fprintf(stderr, "hidden");
		fm->italic_angle = 0.0; /* FreeType hides the angle */
	}

	if (FT_Get_Sfnt_Name(f, TT_NAME_ID_COPYRIGHT, &sn)) {
		fm->name_copyright = strdup("");
	} else {
		fm->name_copyright = strndup((const char*)sn.string, 
				sn.string_len);
	}

	fm->name_family = f->family_name;

	fm->name_style = f->style_name;
	if (fm->name_style == NULL)
		fm->name_style = (char *) "";

	if (FT_Get_Sfnt_Name(f, TT_NAME_ID_FULL_NAME, &sn)) {
		int len;

		len = strlen(fm->name_family) + strlen(fm->name_style) + 2;
		if ((fm->name_full = malloc(len)) == NULL) {
			fprintf(stderr, "****malloc failed %s line %d\n", 
					__FILE__, __LINE__);
			return 1;
		}
		strcpy(fm->name_full, fm->name_family);
		if (strlen(fm->name_style) != 0) {
			strcat(fm->name_full, " ");
			strcat(fm->name_full, fm->name_style);
		}
	} else
		fm->name_full = strndup((const char*)sn.string, sn.string_len);

	if (FT_Get_Sfnt_Name(f, TT_NAME_ID_VERSION_STRING, &sn)) {
		fm->name_version = strdup("1.0");
	} else {
		fm->name_version = strndup((const char*)sn.string, 
				sn.string_len);
	}

	if (FT_Get_Sfnt_Name(f, TT_NAME_ID_PS_NAME , &sn)) {
		if ((fm->name_ps = strdup(fm->name_full)) == NULL) {
			fprintf(stderr, "****malloc failed %s line %d\n", 
					__FILE__, __LINE__);
			return 1;
		}
	} else
		fm->name_ps = strndup((const char*)sn.string, sn.string_len);

	for (i = 0; fm->name_ps[i]!=0; i++) {
		/* no spaces in the Postscript name */
		if (fm->name_ps[i] == ' ')
			fm->name_ps[i] = '_';
	}

	/* guess the boldness from the font names */
	fm->force_bold = 0;

	fieldstocheck[0] = fm->name_style;
	fieldstocheck[1] = fm->name_full;
	fieldstocheck[2] = fm->name_ps;

	for (i = 0; !fm->force_bold && i < (int) N_ELEMENTS(fieldstocheck); 
			i++) {
		str = fieldstocheck[i];
		len = strlen(str);

		for (j = 0; j < len; j++) {
			if ((str[j] == 'B' || str[j] == 'b') && 
					(j == 0 || !isalpha(str[j-1])) &&
					!strncmp("old", &str[j+1], 3) &&
					(j + 4 >= len || !islower(str[j+4]))) {
				fm->force_bold=1;
				break;
			}
		}
	}

	return 0;
}

/*
 * Functions to decompose the outlines
 */

static struct glyph *curg;
static struct outline *cur_outline_entry;
static long lastx, lasty;

static int outl_moveto(const FT_Vector *to, void *face)
{
	FT_Face f = (FT_Face) face;
	struct outline *o;

	o = calloc(1, sizeof(struct outline));
	if (!o) {
		fprintf(stderr, "malloc failed\n");
		return 1;
	}

	o->type = MOVE_TO;
	o->data.move_to.x = convert_units(to->x, f->units_per_EM);
	o->data.move_to.y = convert_units(to->y, f->units_per_EM);

	if (cur_outline_entry)
		cur_outline_entry->next = o;
	else
		curg->outline = o;
	cur_outline_entry = o;

	lastx = convert_units(to->x, f->units_per_EM);
	lasty = convert_units(to->y, f->units_per_EM);

	return 0;
}

static int outl_lineto(const FT_Vector *to, void *face)
{
	FT_Face f = (FT_Face) face;
	struct outline *o;

	o = calloc(1, sizeof(struct outline));
	if (!o) {
		fprintf(stderr, "malloc failed\n");
		return 1;
	}

	o->type = LINE_TO;
	o->data.line_to.x = convert_units(to->x, f->units_per_EM);
	o->data.line_to.y = convert_units(to->y, f->units_per_EM);

	if (cur_outline_entry)
		cur_outline_entry->next = o;
	else
		curg->outline = o;
	cur_outline_entry = o;

	lastx = convert_units(to->x, f->units_per_EM);
	lasty = convert_units(to->y, f->units_per_EM);

	return 0;
}

static int outl_conicto(const FT_Vector *control1, const FT_Vector *to, 
		void *face)
{
	FT_Face f = (FT_Face) face;
	struct outline *o;
	double c1x, c1y;

	o = calloc(1, sizeof(struct outline));
	if (!o) {
		fprintf(stderr, "malloc failed\n");
		return 1;
	}

	c1x = (double)lastx + 2.0 *
		((double)convert_units(control1->x, f->units_per_EM) -
		(double)lastx) / 3.0;
	c1y = (double)lasty + 2.0 *
		((double)convert_units(control1->y, f->units_per_EM) -
		(double)lasty) / 3.0;

	o->type = CURVE;
	o->data.curve.x1 = (int)c1x;
	o->data.curve.y1 = (int)c1y;
	o->data.curve.x2 = (int)(c1x +
			((double)convert_units(to->x, f->units_per_EM) -
			(double)lastx) / 3.0);
	o->data.curve.y2 = (int)(c1y +
			((double)convert_units(to->y, f->units_per_EM) -
			(double)lasty) / 3.0);
	o->data.curve.x3 = convert_units(to->x, f->units_per_EM);
	o->data.curve.y3 = convert_units(to->y, f->units_per_EM);

	if (cur_outline_entry)
		cur_outline_entry->next = o;
	else
		curg->outline = o;
	cur_outline_entry = o;

	lastx = convert_units(to->x, f->units_per_EM);
	lasty = convert_units(to->y, f->units_per_EM);

	return 0;
}

static int outl_cubicto(const FT_Vector *control1, const FT_Vector *control2,
		const FT_Vector *to, void *face)
{
	FT_Face f = (FT_Face) face;
	struct outline *o;

	o = calloc(1, sizeof(struct outline));
	if (!o) {
		fprintf(stderr, "malloc failed\n");
		return 1;
	}

	o->type = CURVE;
	o->data.curve.x1 = convert_units(control1->x, f->units_per_EM);
	o->data.curve.y1 = convert_units(control1->y, f->units_per_EM);
	o->data.curve.x2 = convert_units(control2->x, f->units_per_EM);
	o->data.curve.y2 = convert_units(control2->y, f->units_per_EM);
	o->data.curve.x3 = convert_units(to->x, f->units_per_EM);
	o->data.curve.y3 = convert_units(to->y, f->units_per_EM);

	if (cur_outline_entry)
		cur_outline_entry->next = o;
	else
		curg->outline = o;
	cur_outline_entry = o;

	lastx = convert_units(to->x, f->units_per_EM);
	lasty = convert_units(to->y, f->units_per_EM);

	return 0;
}

static FT_Outline_Funcs ft_outl_funcs = {
	outl_moveto,
	outl_lineto,
	outl_conicto,
	outl_cubicto,
	0,
	0
};

/*
 * Get the path of contours for a glyph.
 */

void glpath(ttf2f_ctx *ctx, int glyphno)
{
	FT_Face f = (FT_Face) ctx->face;
	FT_Outline *ol;
	FT_Glyph gly;
	struct outline *o;

	curg = &ctx->glyphs[glyphno];
	cur_outline_entry = 0;

	if (FT_Load_Glyph(f, glyphno, 
		FT_LOAD_NO_BITMAP|FT_LOAD_NO_SCALE|FT_LOAD_NO_HINTING)
			|| f->glyph->format != ft_glyph_format_outline) {
		fprintf(stderr, "Can't load glyph %s, skipped\n", curg->name);
		return;
	}

	ol = &f->glyph->outline;
	lastx = 0; lasty = 0;

	if (FT_Outline_Decompose(ol, &ft_outl_funcs, f)) {
		fprintf(stderr, 
			"Can't decompose outline of glyph %s, skipped\n", 
			curg->name);
		return;
	}

	o = calloc(1, sizeof(struct outline));
	if (!o) {
		fprintf(stderr, "malloc failed\n");
		return;
	}

	o->type = TERMINATE;
	/* todo - handle composite glyphs */
	o->data.terminate.composite = 0;

	if (cur_outline_entry)
		cur_outline_entry->next = o;
	else
		curg->outline = o;
	cur_outline_entry = o;

	if (FT_Get_Glyph(f->glyph, &gly)) {
		fprintf(stderr, "Can't access glyph %s bbox, skipped\n", 
				curg->name);
		return;
	}

	FT_Done_Glyph(gly);
}

#if 0
/*
 * Get the kerning data.
 */

void kerning(ttf2f_ctx *ctx)
{
	int	i, j, n;
	int	nglyphs = face->num_glyphs;
	FT_Vector k;
	struct glyph *gl;

	if (nglyphs == 0 || !FT_HAS_KERNING(face)) {
		fputs("No Kerning data\n", stderr);
		return;
	}

	for (i = 0; i < nglyphs; i++)  {
		if ((glyph_list[i].flags & GF_USED) == 0)
			continue;
		for (j = 0; j < nglyphs; j++) {
			if ((glyph_list[j].flags & GF_USED) == 0)
				continue;
			if (FT_Get_Kerning(face, i, j, ft_kerning_unscaled, &k))
				continue;
			if (k.x == 0)
				continue;

			addkernpair(i, j, k.x);
		}
	}
}

#endif
