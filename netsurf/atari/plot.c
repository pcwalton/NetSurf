/*
 * Copyright 2010 Ole Loots <ole@monochrom.net>
 *
 * This file is part of NetSurf, http://www.netsurf-browser.org/
 *
 * NetSurf is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * NetSurf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <windom.h>

#include "image/bitmap.h"
#include "utils/log.h"
#include "utils/utf8.h"
#include "utils/utils.h"
#include "desktop/gui.h"
#include "desktop/plotters.h"

#include "atari/bitmap.h"
#include "atari/gui.h"
#include "atari/plot.h"
#include "atari/options.h"
#include "desktop/options.h"
#include "atari/plot.h"

GEM_PLOTTER plotter = NULL;
GEM_FONT_PLOTTER fplotter = NULL;

extern APPvar * appv;
extern short vdih;

/*
Init screen and font driver objects.
Returns non-zero value > -1 when the objects could be succesfully created.
Returns value < 0 to indicate an error
*/

int atari_plotter_init( char* drvrname, char * fdrvrname )
{
	GRECT loc_pos={0,0,420,420};
	int err=0;
	struct s_driver_table_entry * drvinfo;
	int flags = 0;

	if( option_atari_dither == 1) 
		flags |= PLOT_FLAG_DITHER;
	if( option_atari_transparency == 1 ) 
		flags |= PLOT_FLAG_TRANS;

	vdih = app.graf.handle;
	if( verbose_log ) {
		dump_vdi_info( vdih ) ;
		dump_plot_drivers();
		dump_font_drivers();
	}

	drvinfo = get_screen_driver_entry( drvrname );

	LOG(("using plotters: %s, %s", drvrname, fdrvrname));
	fplotter = new_font_plotter(vdih, fdrvrname, 0, &err );
	if(err)
		die(("Unable to load font plotter %s -> %s", fdrvrname, plotter_err_str(err) ));

	plotter = new_plotter( vdih, drvrname, &loc_pos, drvinfo->max_bpp,
							flags, fplotter, &err );
	if(err)
		die(("Unable to load graphics plotter %s -> %s", drvrname, plotter_err_str(err) ));

	return( err );
}

int atari_plotter_finalise( void )
{
	delete_plotter( plotter );
	delete_font_plotter( fplotter );
}

bool plot_rectangle( int x0, int y0, int x1, int y1,
							const plot_style_t *style )
{
	plotter->rectangle( plotter, x0, y0, x1, y1, style );
	return ( true );
}

static bool plot_line( int x0, int y0, int x1, int y1,
											const plot_style_t *style )
{
	plotter->line( plotter, x0, y0, x1, y1, style );
	return ( true );
}

static bool plot_polygon(const int *p, unsigned int n,
													const plot_style_t *style)
{
	plotter->polygon( plotter, p, n, style );
	return ( true );
}

bool plot_clip(int x0, int y0, int x1, int y1)
{
	plotter->clip( plotter, x0, y0, x1, y1 );
	return ( true );
}

bool plot_get_clip(struct s_clipping * out){
	out->x0 = plotter->clipping.x0;
	out->y0 = plotter->clipping.y0;
	out->x1 = plotter->clipping.x1;
	out->y1 = plotter->clipping.y1;
	return( true );
}

static bool plot_text(int x, int y, const char *text, size_t length, const plot_font_style_t *fstyle )
{
	plotter->text( plotter, x, y, text, length, fstyle );
	return ( true );
}

static bool plot_disc(int x, int y, int radius, const plot_style_t *style)
{
	plotter->disc(plotter, x, y, radius, style );
	return ( true );
}

static bool plot_arc(int x, int y, int radius, int angle1, int angle2,
	    		const plot_style_t *style)
{
	plotter->arc( plotter, x, y, radius, angle1, angle2, style );
	return ( true );
}

static bool plot_bitmap(int x, int y, int width, int height,
			struct bitmap *bitmap, colour bg,
			bitmap_flags_t flags)
{
	struct bitmap * bm = NULL;
	bool repeat_x = (flags & BITMAPF_REPEAT_X);
	bool repeat_y = (flags & BITMAPF_REPEAT_Y);

	if( option_suppress_images != 0 ) {
		return( true );
	}

	if(  width != bitmap_get_width(bitmap) || height != bitmap_get_height( bitmap) ) {
		assert( plotter->bitmap_resize(plotter, bitmap, width, height ) == 0);
		bm = bitmap->resized;
	} else {
		bm = bitmap;
	}

	/* out of memory? */
	if( bm == NULL )
		return( true );

	if (!(repeat_x || repeat_y)) {
		plotter->bitmap( plotter, bm, x, y, bg, flags );
	} else {
		int xf,yf;
		struct s_clipping clip;
		plotter_get_clip( plotter, &clip );
		int xoff = x;
		int yoff = y;
		/* for now, repeating just works in the rigth / down direction */
		/*
		if( repeat_x == true )
			xoff = clip.x0;
		if(repeat_y == true )
			yoff = clip.y0;
		*/
		for( xf = xoff; xf < clip.x1; xf += width ) {
			for( yf = yoff; yf < clip.y1; yf += height ) {
				plotter->bitmap( plotter, bm, xf, yf, bg, flags );
				if (!repeat_y)
					break;
			}
			if (!repeat_x)
	   			break;
		}
	}
	return ( true );
}

static bool plot_path(const float *p, unsigned int n, colour fill, float width,
											colour c, const float transform[6])
{
	plotter->path( plotter, p, n, fill, width, c, transform );
	return ( true );
}



struct plotter_table plot = {
	.rectangle = plot_rectangle,
	.line = plot_line,
	.polygon = plot_polygon,
	.clip = plot_clip,
	.text = plot_text,
	.disc = plot_disc,
	.arc = plot_arc,
	.bitmap = plot_bitmap,
	.path = plot_path,
	.flush = NULL,
	.group_start = NULL,
	.group_end = NULL,
	/*.option_knockout = false */
	.option_knockout = true
};