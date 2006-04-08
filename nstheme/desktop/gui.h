/*
 * This file is part of NetSurf, http://netsurf.sourceforge.net/
 * Licensed under the GNU General Public License,
 *                http://www.opensource.org/licenses/gpl-license
 * Copyright 2004 Richard Wilson <not_ginger_matt@users.sourceforge.net>
 */

/** \file
 * Interface to platform-specific gui functions.
 */

#ifndef _NSTHEME_DESKTOP_GUI_H_
#define _NSTHEME_DESKTOP_GUI_H_

#include <stdbool.h>

void gui_init(void);
void gui_multitask(void);
void gui_poll(void);
void gui_exit(void);

#endif
