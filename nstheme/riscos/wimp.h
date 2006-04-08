/*
 * This file is part of NetSurf, http://netsurf.sourceforge.net/
 * Licensed under the GNU General Public License,
 *                http://www.opensource.org/licenses/gpl-license
 * Copyright 2004 Richard Wilson <not_ginger_matt@users.sourceforge.net>
 */

/** \file
 * General RISC OS WIMP/OS library functions (interface).
 */


#ifndef _NETSURF_RISCOS_WIMP_H_
#define _NETSURF_RISCOS_WIMP_H_

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "oslib/os.h"
#include "oslib/wimp.h"


#define ro_gui_redraw_icon(w, i) xwimp_set_icon_state(w, i, 0, 0)
char *ro_gui_get_icon_string(wimp_w w, wimp_i i);
void ro_gui_set_icon_string(wimp_w w, wimp_i i, const char *text);
void ro_gui_set_icon_selected_state(wimp_w w, wimp_i i, bool state);
bool ro_gui_get_icon_selected_state(wimp_w w, wimp_i i);
void ro_gui_set_icon_shaded_state(wimp_w w, wimp_i i, bool state);
bool ro_gui_get_icon_shaded_state(wimp_w w, wimp_i i);
int ro_gui_get_icon_background_colour(wimp_w w, wimp_i i);
void ro_gui_set_icon_background_colour(wimp_w w, wimp_i i, int colour);
void ro_gui_set_window_title(wimp_w w, const char *title);
void ro_gui_set_caret_first(wimp_w w);
void ro_gui_open_window_centre(wimp_w w);

osspriteop_area *ro_gui_load_sprite_file(const char *pathname);
#endif
