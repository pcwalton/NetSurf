/*
 * Copyright 2011 John-Mark Bell <jmb@netsurf-browser.org>
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

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "utils/errors.h"

#include "image/image.h"
#include "image/bmp.h"
#include "image/gif.h"
#include "image/ico.h"
#include "image/jpeg.h"
#include "image/mng.h"
#include "image/nssprite.h"
#include "image/png.h"
#include "image/rsvg.h"
#include "image/svg.h"
#include "image/webp.h"

#include "utils/config.h"

/**
 * Initialise image content handlers
 *
 * \return NSERROR_OK on success, appropriate error otherwise.
 */
nserror image_init(void)
{
	nserror error;

#ifdef WITH_BMP
	error = nsbmp_init();
	if (error != NSERROR_OK)
		return error;
#endif

#ifdef WITH_GIF
	error = nsgif_init();
	if (error != NSERROR_OK)
		return error;
#endif

#ifdef WITH_BMP
	error = nsico_init();
	if (error != NSERROR_OK)
		return error;
#endif

#ifdef WITH_JPEG
	error = nsjpeg_init();
	if (error != NSERROR_OK)
		return error;
#endif

#ifdef WITH_MNG
	error = nsmng_init();
	if (error != NSERROR_OK)
		return error;

	error = nsjpng_init();
	if (error != NSERROR_OK)
		return error;
#endif

#ifdef WITH_PNG
	/* Prefer libpng over libmng for pngs by registering later */
	error = nspng_init();
	if (error != NSERROR_OK)
		return error;
#endif

#ifdef WITH_NSSPRITE
	error = nssprite_init();
	if (error != NSERROR_OK)
		return error;
#endif

	/* Prefer rsvg over libsvgtiny for svgs */
#ifdef WITH_NS_SVG
	error = svg_init();
	if (error != NSERROR_OK)
		return error;
#endif
#ifdef WITH_RSVG
	error = nsrsvg_init();
	if (error != NSERROR_OK)
		return error;
#endif

#ifdef WITH_WEBP
	error = webp_init();
	if (error != NSERROR_OK)
		return error;
#endif /* WITH_WEBP */

	return NSERROR_OK;
}

