/*
 * Copyright 2011 Michael Drake <tlsa@netsurf-browser.org>
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

/** \file
 * Core thumbnail handling (implementation).
 */

#include <assert.h>
#include <stdbool.h>

#include "content/content.h"
#include "content/hlcache.h"
#include "desktop/browser.h"
#include "desktop/options.h"
#include "desktop/plotters.h"
#include "desktop/thumbnail.h"
#include "utils/log.h"


/* exported interface, documented in thumbnail.h */
bool thumbnail_redraw(struct hlcache_handle *content,
		int width, int height)
{
	struct rect clip;
	float scale;

	assert(content);

	/* No selection */
	current_redraw_browser = NULL;

	/* Set clip rectangle to required thumbnail size */
	clip.x0 = 0;
	clip.y0 = 0;
	clip.x1 = width;
	clip.y1 = height;

	plot.clip(&clip);

	/* Plot white background */
	plot.rectangle(clip.x0, clip.y0, clip.x1, clip.y1,
			plot_style_fill_white);

	/* Find the scale we're using */
	scale = thumbnail_get_redraw_scale(content, width);

	/* Render the content */
	return content_redraw(content, 0, 0, width, height, &clip, scale,
			0xFFFFFF);	
}


/* exported interface, documented in thumbnail.h */
float thumbnail_get_redraw_scale(struct hlcache_handle *content, int width)
{
	assert(content);

	if (content_get_width(content))
		return (float)width / (float)content_get_width(content);
	else
		return 1.0;	
}