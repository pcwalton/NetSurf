/*
 * Copyright 2006 Richard Wilson <info@tinct.net>
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
 * Content for image/ico (interface).
 */

#ifndef _NETSURF_IMAGE_ICO_H_
#define _NETSURF_IMAGE_ICO_H_

#include "utils/config.h"
#ifdef WITH_BMP

#include <stdbool.h>
#include <libnsbmp.h>

#include "utils/errors.h"

nserror nsico_init(void);
void nsico_fini(void);

#else

#define nsico_init() NSERROR_OK
#define nsico_fini() ((void) 0)

#endif /* WITH_BMP */

#endif
