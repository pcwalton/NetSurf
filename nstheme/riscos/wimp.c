/*
 * This file is part of NetSurf, http://netsurf.sourceforge.net/
 * Licensed under the GNU General Public License,
 *                http://www.opensource.org/licenses/gpl-license
 * Copyright 2004 Richard Wilson <not_ginger_matt@users.sourceforge.net>
 */

/** \file
 * General RISC OS WIMP/OS library functions (implementation).
 */

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "oslib/os.h"
#include "oslib/osfile.h"
#include "oslib/wimp.h"
#include "oslib/wimpextend.h"
#include "oslib/wimpreadsysinfo.h"
#include "oslib/wimpspriteop.h"
#include "nstheme/desktop/gui.h"
#include "nstheme/riscos/gui.h"
#include "nstheme/riscos/wimp.h"
#include "nstheme/utils/log.h"
#include "nstheme/utils/utils.h"


/**
 * Read the contents of an icon.
 *
 * \param  w  window handle
 * \param  i  icon handle
 * \return string in icon
 */
char *ro_gui_get_icon_string(wimp_w w, wimp_i i) {
  	char *text;
  	int size;
	wimp_icon_state ic;
	ic.w = w;
	ic.i = i;
	if (xwimp_get_icon_state(&ic)) return NULL;
	text = ic.icon.data.indirected_text.text;
	size = ic.icon.data.indirected_text.size;
	text[size - 1] = '\0';
	while (text[0]) {
		if (text[0] < 32) {
			text[0] = '\0';
		} else {
			text++;
		}
	} 
	return ic.icon.data.indirected_text.text;
}


/**
 * Set the contents of an icon to a string.
 *
 * \param  w     window handle
 * \param  i     icon handle
 * \param  text  string (copied)
 */
void ro_gui_set_icon_string(wimp_w w, wimp_i i, const char *text) {
	wimp_caret caret;
	wimp_icon_state ic;
	int old_len, len;

	/*	Get the icon data
	*/
	ic.w = w;
	ic.i = i;
	if (xwimp_get_icon_state(&ic))
		return;

	/*	Check that the existing text is not the same as the updated text
		to stop flicker
	*/
	if (ic.icon.data.indirected_text.size
	    && !strncmp(ic.icon.data.indirected_text.text, text,
			(unsigned int)ic.icon.data.indirected_text.size - 1))
		return;

	/*	Copy the text across
	*/
	old_len = strlen(ic.icon.data.indirected_text.text);
	if (ic.icon.data.indirected_text.size) {
		strncpy(ic.icon.data.indirected_text.text, text,
			(unsigned int)ic.icon.data.indirected_text.size - 1);
		ic.icon.data.indirected_text.text[ic.icon.data.indirected_text.size - 1] = '\0';
	}

	/*	Handle the caret being in the icon
	*/
	if (!xwimp_get_caret_position(&caret)) {
		if ((caret.w == w) && (caret.i == i)) {
		  	len = strlen(text);
		  	if ((caret.index > len) || (caret.index == old_len)) caret.index = len;
			xwimp_set_caret_position(w, i, caret.pos.x, caret.pos.y, -1, caret.index);
		}
	}

	/*	Redraw the icon
	*/
	ro_gui_redraw_icon(w, i);
}


/**
 * Set the selected state of an icon.
 *
 * \param  w     window handle
 * \param  i     icon handle
 * \param  state selected state
 */
void ro_gui_set_icon_selected_state(wimp_w w, wimp_i i, bool state) {
	os_error *error;
	if (ro_gui_get_icon_selected_state(w, i) == state) return;
	error = xwimp_set_icon_state(w, i,
			(state ? wimp_ICON_SELECTED : 0), wimp_ICON_SELECTED);
	if (error) {
		LOG(("xwimp_get_icon_state: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("WimpError", error->errmess);
	}
}

/**
 * Gets the selected state of an icon.
 *
 * \param  w     window handle
 * \param  i     icon handle
 */
bool ro_gui_get_icon_selected_state(wimp_w w, wimp_i i) {
	os_error *error;
	wimp_icon_state ic;
	ic.w = w;
	ic.i = i;
	error = xwimp_get_icon_state(&ic);
	if (error) {
		LOG(("xwimp_get_icon_state: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("WimpError", error->errmess);
		return false;
	}
	return ((ic.icon.flags & wimp_ICON_SELECTED) != 0);
}


/**
 * Set the shaded state of an icon.
 *
 * \param  w     window handle
 * \param  i     icon handle
 * \param  state selected state
 */
void ro_gui_set_icon_shaded_state(wimp_w w, wimp_i i, bool state) {
	os_error *error;
	if (ro_gui_get_icon_shaded_state(w, i) == state) return;
	error = xwimp_set_icon_state(w, i,
			(state ? wimp_ICON_SHADED : 0), wimp_ICON_SHADED);
	if (error) {
		LOG(("xwimp_get_icon_state: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("WimpError", error->errmess);
	}
}


/**
 * Gets the shaded state of an icon.
 *
 * \param  w     window handle
 * \param  i     icon handle
 */
bool ro_gui_get_icon_shaded_state(wimp_w w, wimp_i i) {
	wimp_icon_state ic;
	ic.w = w;
	ic.i = i;
	xwimp_get_icon_state(&ic);
	return (ic.icon.flags & wimp_ICON_SHADED) != 0;
}


/**
 * Set the background colour of an icon.
 *
 * \param  w     window handle
 * \param  i     icon handle
 * \param  state selected state
 */
void ro_gui_set_icon_background_colour(wimp_w w, wimp_i i, int colour) {
	os_error *error;
	if (ro_gui_get_icon_background_colour(w, i) == colour) return;
	error = xwimp_set_icon_state(w, i,
			(colour <<wimp_ICON_BG_COLOUR_SHIFT), (15 << wimp_ICON_BG_COLOUR_SHIFT));
	if (error) {
		LOG(("xwimp_get_icon_state: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("WimpError", error->errmess);
	}
}


/**
 * Gets the background colour of an icon.
 *
 * \param  w     window handle
 * \param  i     icon handle
 */
int ro_gui_get_icon_background_colour(wimp_w w, wimp_i i) {
	os_error *error;
	wimp_icon_state ic;
	ic.w = w;
	ic.i = i;
	error = xwimp_get_icon_state(&ic);
	if (error) {
		LOG(("xwimp_get_icon_state: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("WimpError", error->errmess);
		return -1;
	}
	return ((ic.icon.flags >> wimp_ICON_BG_COLOUR_SHIFT) & 15);
}


/**
 * Set a window title (does *not* redraw the title)
 *
 * \param  w     window handle
 * \param  text  new title (copied)
 */
void ro_gui_set_window_title(wimp_w w, const char *text) {
	wimp_window_info_base window;
	os_error *error;

	/*	Get the window details
	*/
	window.w = w;
	error = xwimp_get_window_info_header_only((wimp_window_info *)&window);
	if (error) {
		LOG(("xwimp_get_window_info: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("WimpError", error->errmess);
		return;
	}

	/*	Set the title string
	*/
	strncpy(window.title_data.indirected_text.text, text,
			(unsigned int)window.title_data.indirected_text.size - 1);
	window.title_data.indirected_text.text[window.title_data.indirected_text.size - 1] = '\0';
}


/**
 * Places the caret in the first available icon
 */
void ro_gui_set_caret_first(wimp_w w) {
  	int icon, button;
	wimp_window_state win_state;
	wimp_window_info_base window;
	wimp_icon_state state;
	wimp_caret caret;
	os_error *error;

	/*	Check the window is open
	*/
	win_state.w = w;
	error = xwimp_get_window_state(&win_state);
	if (error) {
		LOG(("xwimp_get_window_state: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("WimpError", error->errmess);
		return;
	}
	if (!(win_state.flags & wimp_WINDOW_OPEN)) return;

	/*	Check if the window already has the caret
	*/
	if (!xwimp_get_caret_position(&caret)) {
		if (caret.w == w) return;
	}
	
	/*	Get the window details
	*/
	window.w = w;
	error = xwimp_get_window_info_header_only((wimp_window_info *)&window);
	if (error) {
		LOG(("xwimp_get_window_info: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("WimpError", error->errmess);
		return;
	}

	/*	Work through our icons
	*/
	state.w = w;
	for (icon = 0; icon < window.icon_count; icon++) {
	  	/*	Get the icon state
	  	*/
		state.i = icon;
		error = xwimp_get_icon_state(&state);
		if (error) {
			LOG(("xwimp_get_icon_state: 0x%x: %s",
					error->errnum, error->errmess));
			warn_user("WimpError", error->errmess);
			return;
		}

		/*	Ignore if it's shaded
		*/
		if (state.icon.flags & wimp_ICON_SHADED)
			continue;

		/*	Check if it's writable
		*/
		button = (state.icon.flags >> wimp_ICON_BUTTON_TYPE_SHIFT) & 0xf;
		if ((button == wimp_BUTTON_WRITE_CLICK_DRAG) ||
				(button == wimp_BUTTON_WRITABLE)) {
			error = xwimp_set_caret_position(w, icon, 0, 0, -1,
					strlen(state.icon.data.indirected_text.text));
			if (error) {
				LOG(("xwimp_set_caret_position: 0x%x: %s",
						error->errnum, error->errmess));
				warn_user("WimpError", error->errmess);
			}
			return;
		}
	}
}


/**
 * Opens a window at the centre of either another window or the screen
 *
 * /param child the child window
 */
void ro_gui_open_window_centre(wimp_w w) {
	os_error *error;
	wimp_window_state state;
	int mid_x, mid_y, dimension;

	state.w = w;
	error = xwimp_get_window_state(&state);
	if (error) {
		warn_user("WimpError", error->errmess);
		return;
	}
	if (!(state.flags & wimp_WINDOW_OPEN)) {
		ro_gui_screen_size(&mid_x, &mid_y);
		dimension = state.visible.x1 - state.visible.x0;
		state.visible.x0 = (mid_x - dimension) >> 1;
		state.visible.x1 = state.visible.x0 + dimension;
		dimension = state.visible.y1 - state.visible.y0;
		state.visible.y0 = (mid_y - dimension) >> 1;
		state.visible.y1 = state.visible.y0 + dimension;
	}
	state.next = wimp_TOP;
	wimp_open_window((wimp_open *)&state);
}



/**
 * Load a sprite file into memory.
 *
 * \param  pathname  file to load
 * \return  sprite area, or 0 on memory exhaustion or error and error reported
 */

osspriteop_area *ro_gui_load_sprite_file(const char *pathname)
{
	int len;
	fileswitch_object_type obj_type;
	osspriteop_area *area;
	os_error *error;

	error = xosfile_read_stamped_no_path(pathname,
			&obj_type, 0, 0, &len, 0, 0);
	if (error) {
		LOG(("xosfile_read_stamped_no_path: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("MiscError", error->errmess);
		return 0;
	}
	if (obj_type != fileswitch_IS_FILE) {
		warn_user("FileError", pathname);
		return 0;
	}

	area = malloc(len + 4);
	if (!area) {
		warn_user("NoMemory", 0);
		return 0;
	}

	area->size = len + 4;
	area->sprite_count = 0;
	area->first = 16;
	area->used = 16;

	error = xosspriteop_load_sprite_file(osspriteop_USER_AREA,
			area, pathname);
	if (error) {
		LOG(("xosspriteop_load_sprite_file: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("MiscError", error->errmess);
		free(area);
		return 0;
	}

	return area;
}
