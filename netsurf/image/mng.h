/*
 * Copyright 2004 Richard Wilson <not_ginger_matt@users.sourceforge.net>
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
 * Content for image/mng, image/png, and image/jng (interface).
 */

#ifndef _NETSURF_IMAGE_MNG_H_
#define _NETSURF_IMAGE_MNG_H_

#include "utils/config.h"
#ifdef WITH_MNG

#include <stdbool.h>

nserror nsmng_init(void);
void nsmng_fini(void);
nserror nsjpng_init(void);
void nsjpng_fini(void);

#else

#define nsmng_init() NSERROR_OK
#define nsmng_fini() ((void) 0)
#define nsjpng_init() NSERROR_OK
#define nsjpng_fini() ((void) 0)

#endif /* WITH_MNG */

#endif
