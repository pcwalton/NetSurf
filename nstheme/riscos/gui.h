/*
 * This file is part of NetSurf, http://netsurf.sourceforge.net/
 * Licensed under the GNU General Public License,
 *                http://www.opensource.org/licenses/gpl-license
 * Copyright 2003 Phil Mellor <monkeyson@users.sourceforge.net>
 * Copyright 2004 James Bursa <bursa@users.sourceforge.net>
 * Copyright 2004 Andrew Timmins <atimmins@blueyonder.co.uk>
 */

#ifndef _NETSURF_RISCOS_GUI_H_
#define _NETSURF_RISCOS_GUI_H_

#include <stdbool.h>
#include <stdlib.h>
#include "oslib/osspriteop.h"
#include "oslib/wimp.h"
#include "nstheme/desktop/nstheme.h"
#include "nstheme/desktop/gui.h"
#include "nstheme/desktop/options.h"

#define THEMES_DIR "<NSTheme$Dir>.Themes"

extern char *theme_filename;
extern char *sprite_filename;
extern osspriteop_area *theme_sprites;
extern wimp_w dialog_info, dialog_main, dialog_saveas, dialog_warning;
extern wimp_menu *iconbar_menu, *main_menu, *colour_menu;
extern int iconbar_menu_height;
extern wimp_menu *current_menu;

struct theme_file_header {
	unsigned int magic_value;
	unsigned int parser_version;
	char name[32];
	char author[64];
	char browser_bg;
	char hotlist_bg;
	char status_bg;
	char status_fg;
	char theme_flags;
	char future_expansion_1;
	char future_expansion_2;
	char future_expansion_3;
	unsigned int compressed_sprite_size;
	unsigned int decompressed_sprite_size;
};

/* in gui.c */
void ro_gui_screen_size(int *width, int *height);

/* in menus.c */
void ro_gui_menus_init(void);
void ro_gui_create_menu(wimp_menu* menu, int x, int y);
void ro_gui_popup_menu(wimp_menu *menu, wimp_w w, wimp_i i);
void ro_gui_menu_selection(wimp_selection* selection);
void ro_gui_menu_warning(wimp_message_menu_warning *warning);

/* in dialog.c */
void ro_gui_dialog_init(void);
void ro_gui_dialog_open(wimp_w w);
void ro_gui_dialog_close(wimp_w close);
void ro_gui_dialog_click(wimp_pointer *pointer);
bool ro_gui_dialog_keypress(wimp_key *key);
void ro_gui_dialog_prepare_main(void);

/* in save.c */
void ro_gui_save_open(int save_type, int x, int y);
void ro_gui_save_click(wimp_pointer *pointer);
void ro_gui_save_drag_end(wimp_dragged *drag);
void ro_gui_save_datasave_ack(wimp_message *message);
bool ro_gui_load_theme(char *path);

#define ICON_WARNING_MESSAGE 0
#define ICON_WARNING_CONTINUE 1
#define ICON_WARNING_HELP 2

#define ICON_MAIN_NAME 3
#define ICON_MAIN_AUTHOR 5
#define ICON_MAIN_THROBBER_LEFT 8
#define ICON_MAIN_THROBBER_REDRAW 9
#define ICON_MAIN_REPORT 10
#define ICON_MAIN_REMOVE 11
#define ICON_MAIN_BROWSER_COLOUR 15
#define ICON_MAIN_BROWSER_MENU 16
#define ICON_MAIN_HOTLIST_COLOUR 18
#define ICON_MAIN_HOTLIST_MENU 19
#define ICON_MAIN_STATUSBG_COLOUR 21
#define ICON_MAIN_STATUSBG_MENU 22
#define ICON_MAIN_STATUSFG_COLOUR 24
#define ICON_MAIN_STATUSFG_MENU 25

#define ICON_SAVE_ICON 0
#define ICON_SAVE_PATH 1
#define ICON_SAVE_OK 2
#define ICON_SAVE_CANCEL 3

#endif
