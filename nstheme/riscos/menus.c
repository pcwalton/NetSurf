/*
 * This file is part of NetSurf, http://netsurf.sourceforge.net/
 * Licensed under the GNU General Public License,
 *		  http://www.opensource.org/licenses/gpl-license
 * Copyright 2003 Phil Mellor <monkeyson@users.sourceforge.net>
 * Copyright 2004 James Bursa <bursa@users.sourceforge.net>
 * Copyright 2003 John M Bell <jmb202@ecs.soton.ac.uk>
 * Copyright 2004 Richard Wilson <not_ginger_matt@users.sourceforge.net>
 */

/** \file
 * Menu creation and handling (implementation).
 */

#include <stdlib.h>
#include <string.h>
#include "oslib/os.h"
#include "oslib/osgbpb.h"
#include "oslib/wimp.h"
#include "nstheme/desktop/gui.h"
#include "nstheme/riscos/gui.h"
#include "nstheme/riscos/help.h"
#include "nstheme/riscos/options.h"
#include "nstheme/riscos/wimp.h"
#include "nstheme/utils/log.h"
#include "nstheme/utils/messages.h"
#include "nstheme/utils/utils.h"


static void translate_menu(wimp_menu *menu);


wimp_menu *current_menu;
wimp_i menu_icon;

/*	Default menu item flags
*/
#define COLOUR_FLAGS (wimp_ICON_TEXT | wimp_ICON_FILLED | \
		(wimp_COLOUR_BLACK << wimp_ICON_FG_COLOUR_SHIFT))
#define DEFAULT_FLAGS (wimp_ICON_TEXT | wimp_ICON_FILLED | \
		(wimp_COLOUR_BLACK << wimp_ICON_FG_COLOUR_SHIFT) | \
		(wimp_COLOUR_WHITE << wimp_ICON_BG_COLOUR_SHIFT))


/*	Iconbar menu
*/
static wimp_MENU(4) ibar_menu = {
  { "NSTheme" }, 7,2,7,0, 200, 44, 0,
  {
    { 0,	      wimp_NO_SUB_MENU, DEFAULT_FLAGS, { "Info" } },
    { 0,	      wimp_NO_SUB_MENU, DEFAULT_FLAGS, { "AppHelp" } },
    { wimp_MENU_LAST, wimp_NO_SUB_MENU, DEFAULT_FLAGS, { "Quit" } }
  }
};
int iconbar_menu_height = 3 * 44;
wimp_menu *iconbar_menu = (wimp_menu *)&ibar_menu;


/*	Colour menu
*/
static wimp_MENU(16) col_menu = {
  { "Colour" }, 7,2,7,0, 200, 44, 0,
  {
    { 0,	      wimp_NO_SUB_MENU, COLOUR_FLAGS |
			(wimp_COLOUR_WHITE << wimp_ICON_BG_COLOUR_SHIFT), { "" } },
    { 0,	      wimp_NO_SUB_MENU, COLOUR_FLAGS |
			(wimp_COLOUR_VERY_LIGHT_GREY << wimp_ICON_BG_COLOUR_SHIFT), { "" } },
    { 0,	      wimp_NO_SUB_MENU, COLOUR_FLAGS |
			(wimp_COLOUR_LIGHT_GREY << wimp_ICON_BG_COLOUR_SHIFT), { "" } },
    { 0,	      wimp_NO_SUB_MENU, COLOUR_FLAGS |
			(wimp_COLOUR_MID_LIGHT_GREY << wimp_ICON_BG_COLOUR_SHIFT), { "" } },
    { 0,	      wimp_NO_SUB_MENU, COLOUR_FLAGS |
			(wimp_COLOUR_MID_DARK_GREY << wimp_ICON_BG_COLOUR_SHIFT), { "" } },
    { 0,	      wimp_NO_SUB_MENU, COLOUR_FLAGS |
			(wimp_COLOUR_DARK_GREY << wimp_ICON_BG_COLOUR_SHIFT), { "" } },
    { 0,	      wimp_NO_SUB_MENU, COLOUR_FLAGS |
			(wimp_COLOUR_VERY_DARK_GREY << wimp_ICON_BG_COLOUR_SHIFT), { "" } },
    { 0,	      wimp_NO_SUB_MENU, COLOUR_FLAGS |
			(wimp_COLOUR_BLACK << wimp_ICON_BG_COLOUR_SHIFT), { "" } },
    { 0,	      wimp_NO_SUB_MENU, COLOUR_FLAGS |
			(wimp_COLOUR_DARK_BLUE << wimp_ICON_BG_COLOUR_SHIFT), { "" } },
    { 0,	      wimp_NO_SUB_MENU, COLOUR_FLAGS |
			(wimp_COLOUR_YELLOW << wimp_ICON_BG_COLOUR_SHIFT), { "" } },
    { 0,	      wimp_NO_SUB_MENU, COLOUR_FLAGS |
			(wimp_COLOUR_LIGHT_GREEN << wimp_ICON_BG_COLOUR_SHIFT), { "" } },
    { 0,	      wimp_NO_SUB_MENU, COLOUR_FLAGS |
			(wimp_COLOUR_RED << wimp_ICON_BG_COLOUR_SHIFT), { "" } },
    { 0,	      wimp_NO_SUB_MENU, COLOUR_FLAGS |
			(wimp_COLOUR_CREAM << wimp_ICON_BG_COLOUR_SHIFT), { "" } },
    { 0,	      wimp_NO_SUB_MENU, COLOUR_FLAGS |
			(wimp_COLOUR_DARK_GREEN << wimp_ICON_BG_COLOUR_SHIFT), { "" } },
    { 0,	      wimp_NO_SUB_MENU, COLOUR_FLAGS |
			(wimp_COLOUR_ORANGE << wimp_ICON_BG_COLOUR_SHIFT), { "" } },
    { wimp_MENU_LAST, wimp_NO_SUB_MENU, COLOUR_FLAGS |
			(wimp_COLOUR_LIGHT_BLUE << wimp_ICON_BG_COLOUR_SHIFT), { "" } }
  }
};
wimp_menu *colour_menu = (wimp_menu *)&col_menu;

/*	Main browser menu
*/
static wimp_MENU(3) menu = {
  { "NSTheme" }, 7,2,7,0, 200, 44, 0,
  {
    { wimp_MENU_GIVE_WARNING, wimp_NO_SUB_MENU, DEFAULT_FLAGS, { "SaveAs" } },
    { wimp_MENU_GIVE_WARNING, wimp_NO_SUB_MENU, DEFAULT_FLAGS, { "Export" } },
    { wimp_MENU_LAST,         wimp_NO_SUB_MENU, DEFAULT_FLAGS, { "AppHelp" } }
  }
};
wimp_menu *main_menu = (wimp_menu *) &menu;


/**
 * Create menu structures.
 */

void ro_gui_menus_init(void)
{
	translate_menu(iconbar_menu);
	translate_menu(main_menu);
	translate_menu(colour_menu);

	iconbar_menu->entries[0].sub_menu = (wimp_menu *)dialog_info;
	main_menu->entries[0].sub_menu = (wimp_menu *)dialog_saveas;
	main_menu->entries[1].sub_menu = (wimp_menu *)dialog_saveas;
}


/**
 * Replace text in a menu with message values.
 */

void translate_menu(wimp_menu *menu)
{
	unsigned int i = 0;
	const char *indirected_text;

	/*	We can't just blindly set something as indirected as if we use
		the fallback messages text (ie the pointer we gave), we overwrite
		this data when setting the pointer to the indirected text we
		already had.
	*/
	indirected_text = messages_get(menu->title_data.text);
	if (indirected_text != menu->title_data.text) {
		menu->title_data.indirected_text.text = indirected_text;
		menu->entries[0].menu_flags |= wimp_MENU_TITLE_INDIRECTED;

	}

	/* items */
	do {
		indirected_text = messages_get(menu->entries[i].data.text);
		if (indirected_text != menu->entries[i].data.text) {
			menu->entries[i].icon_flags |= wimp_ICON_INDIRECTED;
			menu->entries[i].data.indirected_text.text = indirected_text;
			menu->entries[i].data.indirected_text.validation = 0;
			menu->entries[i].data.indirected_text.size = strlen(indirected_text) + 1;
		}
		i++;
	} while ((menu->entries[i - 1].menu_flags & wimp_MENU_LAST) == 0);
}


/**
 * Display a menu.
 */

void ro_gui_create_menu(wimp_menu *menu, int x, int y) {
	os_error *error;
	current_menu = menu;
	int colour;
	
	if (menu == colour_menu) {
		colour = ro_gui_get_icon_background_colour(dialog_main, menu_icon - 1);
		for (int i = 0; i < 16; i++) {
			if (i == colour) {
				colour_menu->entries[i].menu_flags |= wimp_MENU_TICKED;
			} else {
				colour_menu->entries[i].menu_flags &= ~wimp_MENU_TICKED;
			}
		}  
	} else if (menu == main_menu) {
		if (theme_sprites) {
			main_menu->entries[1].icon_flags &= ~wimp_ICON_SHADED;
		} else {
			main_menu->entries[1].icon_flags |= wimp_ICON_SHADED;
		} 
	}

	error = xwimp_create_menu(menu, x - 64, y);
	if (error) {
		LOG(("xwimp_create_menu: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("MenuError", error->errmess);
	}
}


/**
 * Display a pop-up menu next to the specified icon.
 */

void ro_gui_popup_menu(wimp_menu *menu, wimp_w w, wimp_i i) {
	wimp_window_state state;
	wimp_icon_state icon_state;
	state.w = w;
	icon_state.w = w;
	icon_state.i = i;
	wimp_get_window_state(&state);
	wimp_get_icon_state(&icon_state);
	menu_icon = i;
	ro_gui_create_menu(menu,
			state.visible.x0 + icon_state.icon.extent.x1 + 64,
			state.visible.y1 + icon_state.icon.extent.y1);
}


/**
 * Handle menu selection.
 */

void ro_gui_menu_selection(wimp_selection *selection) {
	wimp_pointer pointer;

	wimp_get_pointer_info(&pointer);

	if (current_menu == iconbar_menu) {
		switch (selection->items[0]) {
			case 0: /* Info */
				ro_gui_create_menu((wimp_menu *) dialog_info,
						pointer.pos.x, pointer.pos.y);
				break;
			case 1: /* Help */
				xos_cli("Filer_Run <NSTheme$Dir>.!Help");
				break;
			case 2: /* Quit */
				application_quit = true;
				break;
		}

	} else if (current_menu == main_menu) {
		switch (selection->items[0]) {
			case 2:	/* Help */
				xos_cli("Filer_Run <NSTheme$Dir>.!Help");
				break;
		}
	} else if (current_menu == colour_menu) {
		ro_gui_set_icon_background_colour(dialog_main, menu_icon - 1,
				selection->items[0]);
	}

	if (pointer.buttons == wimp_CLICK_ADJUST) {
		ro_gui_create_menu(current_menu, 0, 0);
	}

}

/**
 * Handle Message_MenuWarning.
 */

void ro_gui_menu_warning(wimp_message_menu_warning *warning) {
	if (current_menu != main_menu) return;
	switch (warning->selection.items[0]) {
	  	case 0:
	  		ro_gui_save_open(1, warning->pos.x, warning->pos.y);
	  		break;
	  	case 1:
	  		ro_gui_save_open(2, warning->pos.x, warning->pos.y);
	  		break;
	}	
}
