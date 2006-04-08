/*
 * This file is part of NetSurf, http://netsurf.sourceforge.net/
 * Licensed under the GNU General Public License,
 *		  http://www.opensource.org/licenses/gpl-license
 * Copyright 2003 Phil Mellor <monkeyson@users.sourceforge.net>
 * Copyright 2004 James Bursa <bursa@users.sourceforge.net>
 * Copyright 2003 John M Bell <jmb202@ecs.soton.ac.uk>
 * Copyright 2004 Richard Wilson <not_ginger_matt@users.sourceforge.net>
 * Copyright 2004 Andrew Timmins <atimmins@blueyonder.co.uk>
 */

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "oslib/colourtrans.h"
#include "oslib/os.h"
#include "oslib/osfile.h"
#include "oslib/osgbpb.h"
#include "oslib/osspriteop.h"
#include "oslib/wimp.h"
#include "nstheme/desktop/nstheme.h"
#include "nstheme/riscos/gui.h"
#include "nstheme/riscos/options.h"
#include "nstheme/riscos/wimp.h"
#include "nstheme/utils/log.h"
#include "nstheme/utils/messages.h"
#include "nstheme/utils/utils.h"


osspriteop_area *theme_sprites = NULL;

const char *theme_sprite_name[] = {
	"back", "pback",
	"forward", "pforward",
	"stop", "pstop",
	"reload", "preload",
	"home", "phome",
	"search", "psearch",
	"history", "phistory",
	"scale", "pscale",
	"hotlist", "photlist",
	"save", "psave",
	"print", "pprint",
	"create", "pcreate",
	"delete", "pdelete",
	"launch", "plaunch",
	"open", "popen",
	"expand", "pexpand",
	"separator",
	NULL
};


wimp_w dialog_info, dialog_main, dialog_saveas, dialog_warning;

static void ro_gui_dialog_click_main(wimp_pointer *pointer);
static void ro_gui_dialog_click_warning(wimp_pointer *pointer);
static wimp_w ro_gui_dialog_create(const char *template_name);
static wimp_window * ro_gui_dialog_load_template(const char *template_name);
static void ro_gui_dialog_main_report(void);
static unsigned int ro_gui_dialog_test_sprite(const char *name);


/**
 * Load and create dialogs from template file.
 */

void ro_gui_dialog_init(void)
{
	dialog_info = ro_gui_dialog_create("info");
	dialog_main = ro_gui_dialog_create("main");
	dialog_saveas = ro_gui_dialog_create("saveas");
	dialog_warning = ro_gui_dialog_create("warning");
	ro_gui_set_icon_string(dialog_main, ICON_MAIN_NAME, "\0");
	ro_gui_set_icon_string(dialog_main, ICON_MAIN_AUTHOR, "\0");
}


/**
 * Load a template without creating a window.
 *
 * \param  template_name  name of template to load
 * \return  window block
 *
 * Exits through die() on error.
 */

wimp_window * ro_gui_dialog_load_template(const char *template_name)
{
	char name[20];
	int context, window_size, data_size;
	char *data;
	wimp_window *window;
	os_error *error;

	/* wimp_load_template won't accept a const char * */
	strncpy(name, template_name, sizeof name);

	/* find required buffer sizes */
	error = xwimp_load_template(wimp_GET_SIZE, 0, 0, wimp_NO_FONTS,
			name, 0, &window_size, &data_size, &context);
	if (error) {
		LOG(("xwimp_load_template: 0x%x: %s",
				error->errnum, error->errmess));
		xwimp_close_template();
		die(error->errmess);
	}
	if (!context) {
		LOG(("template '%s' missing", template_name));
		xwimp_close_template();
		die("Template");
	}

	/* allocate space for indirected data and temporary window buffer */
	data = malloc(data_size);
	window = malloc(window_size);
	if (!data || !window) {
		xwimp_close_template();
		die("NoMemory");
	}

	/* load template */
	error = xwimp_load_template(window, data, data + data_size,
			wimp_NO_FONTS, name, 0, 0, 0, 0);
	if (error) {
		LOG(("xwimp_load_template: 0x%x: %s",
				error->errnum, error->errmess));
		xwimp_close_template();
		die(error->errmess);
	}

	return window;
}


/**
 * Create a window from a template.
 *
 * \param  template_name  name of template to load
 * \return  window handle
 *
 * Exits through die() on error.
 */

wimp_w ro_gui_dialog_create(const char *template_name)
{
	wimp_window *window;
	wimp_w w;
	os_error *error;

	window = ro_gui_dialog_load_template(template_name);

	/* create window */
	error = xwimp_create_window(window, &w);
	if (error) {
		LOG(("xwimp_create_window: 0x%x: %s",
				error->errnum, error->errmess));
		xwimp_close_template();
		die(error->errmess);
	}

	/* the window definition is copied by the wimp and may be freed */
	free(window);

	return w;
}


/**
 * Open a dialog box, centered on the screen.
 */

void ro_gui_dialog_open(wimp_w w)
{
	int screen_x, screen_y, dx, dy;
	wimp_window_state open;
	os_error *error;

	/* find screen centre in os units */
	ro_gui_screen_size(&screen_x, &screen_y);
	screen_x /= 2;
	screen_y /= 2;

	/* centre and open */
	open.w = w;
	error = xwimp_get_window_state(&open);
	if (error) {
		LOG(("xwimp_get_window_state: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("WimpError", error->errmess);
		return;
	}
	dx = (open.visible.x1 - open.visible.x0) / 2;
	dy = (open.visible.y1 - open.visible.y0) / 2;
	open.visible.x0 = screen_x - dx;
	open.visible.x1 = screen_x + dx;
	open.visible.y0 = screen_y - dy;
	open.visible.y1 = screen_y + dy;
	open.next = wimp_TOP;
	error = xwimp_open_window((wimp_open *) &open);
	if (error) {
		LOG(("xwimp_open_window: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("WimpError", error->errmess);
		return;
	}

	/*	Set the caret position
	*/
	ro_gui_set_caret_first(w);
}




/**
 * Handle key presses in one of the dialog boxes.
 */

bool ro_gui_dialog_keypress(wimp_key *key) {
	if (key->c == wimp_KEY_ESCAPE) {
		ro_gui_dialog_close(key->w);
		return true;
	} else if (key->c == wimp_KEY_RETURN) {
/*		if ((key->w == dialog_folder) || (key->w == dialog_entry)) {
			return true;
		}
*/	}
	return false;
}


/**
 * Handle clicks in one of the dialog boxes.
 */

void ro_gui_dialog_click(wimp_pointer *pointer)
{
	if (pointer->buttons == wimp_CLICK_MENU) {
		if (pointer->w == dialog_main) {
			ro_gui_create_menu(main_menu, pointer->pos.x,
					   pointer->pos.y);
		}
	  	return;
	}

	if (pointer->w == dialog_main)
		ro_gui_dialog_click_main(pointer);
	else if (pointer->w == dialog_warning)
		ro_gui_dialog_click_warning(pointer);
}


/**
 * Handle clicks in the main dialog.
 */

void ro_gui_dialog_click_main(wimp_pointer *pointer) {
	if (pointer->i == ICON_MAIN_BROWSER_MENU) {
		ro_gui_popup_menu(colour_menu, dialog_main,
				ICON_MAIN_BROWSER_MENU);
	} else if (pointer->i == ICON_MAIN_HOTLIST_MENU) {
		ro_gui_popup_menu(colour_menu, dialog_main,
				ICON_MAIN_HOTLIST_MENU);
	} else if (pointer->i == ICON_MAIN_STATUSBG_MENU) {
		ro_gui_popup_menu(colour_menu, dialog_main,
				ICON_MAIN_STATUSBG_MENU);
	} else if (pointer->i == ICON_MAIN_STATUSFG_MENU) {
		ro_gui_popup_menu(colour_menu, dialog_main,
				ICON_MAIN_STATUSFG_MENU);
	} else if (pointer->i == ICON_MAIN_REPORT) {
	  	ro_gui_dialog_main_report();
	} else if (pointer->i == ICON_MAIN_REMOVE) {
		if (theme_sprites) {
			free(theme_sprites);
			theme_sprites = NULL;
			ro_gui_dialog_prepare_main();
		}
	}
}


/**
 * Handle clicks in the warning dialog.
 */

void ro_gui_dialog_click_warning(wimp_pointer *pointer)
{
	if (pointer->i == ICON_WARNING_CONTINUE)
		ro_gui_dialog_close(dialog_warning);
}


/**
 * Close a dialog box.
 */

void ro_gui_dialog_close(wimp_w close) {
  	os_error *error;
	error = xwimp_close_window(close);
	if (error) {
		LOG(("xwimp_close_window: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("WimpError", error->errmess);
	}
}

void ro_gui_dialog_prepare_main(void) {
	ro_gui_set_icon_shaded_state(dialog_main, ICON_MAIN_REMOVE,
		(theme_sprites == NULL));
	
}

void ro_gui_dialog_main_report(void) {
	char name[16];
	char name2[16];
  	const char *lookup_name;
  	const char *pushed_name;
	unsigned int i;
	int j, n;
	unsigned int err;
  	int warn_count = 0;
  	bool found;
  	int throb_frames = -1;
	FILE *fp;
	fp = fopen("<Wimp$Scrap>", "w");
	if (!fp) {
		warn_user("ReportError", 0);
		LOG(("failed to open file '<Wimp$Scrap>' for writing"));
		return;
	}
	fprintf(fp, messages_get("Title"));
	fprintf(fp, "\n");
	for (i = 0; i < strlen(messages_get("Title")); i++) {
		fprintf(fp, "=");
	}
	fprintf(fp, "\n");
	if (theme_sprites) {
		i = 0;
		while (theme_sprite_name[i]) {
			err = ro_gui_dialog_test_sprite(theme_sprite_name[i]);
			if (err & 1) {
				warn_count++;
				lookup_name = messages_get(theme_sprite_name[i & ~1]);
				if (i & 1) {
					pushed_name = messages_get("pushed");
				} else {
				  	pushed_name = "";
				}
				fprintf(fp, messages_get("WarnNoSpr"),
					theme_sprite_name[i],
					lookup_name, pushed_name);
				fprintf(fp, "\n");
			}
			if (err & 2) {
				warn_count++;
				fprintf(fp, messages_get("WarnHighSpr"),
					theme_sprite_name[i]);
				fprintf(fp, "\n");
			}
			if (err & 4) {
				warn_count++;
				fprintf(fp, messages_get("WarnAlphaSpr"),
					theme_sprite_name[i]);
				fprintf(fp, "\n");
			}
			i++;
		}
		for (j = 1; j <= theme_sprites->sprite_count; j++) {
		  	found = false;
			osspriteop_return_name(osspriteop_USER_AREA,
					theme_sprites, name, 16, j);
			if (strncmp(name, "throbber", 8) == 0) {
				n = atoi(name + 8);
				sprintf(name2, "throbber%i", n);
				if (strcmp(name, name2) == 0) {
					found = true;
					throb_frames = n;
				}
			} else {
				i = 0;
				while (!found && theme_sprite_name[i]) {
					if (strcmp(name, theme_sprite_name[i]) == 0) {
						found = true;
					}
					i++;
			  	}
			}
			
			if (!found) {
				warn_count++;
				fprintf(fp, messages_get("WarnExtraSpr"),
					name);
				fprintf(fp, "\n");
				err = ro_gui_dialog_test_sprite(name);
				if (err & 2) {
					warn_count++;
					fprintf(fp, messages_get("WarnHighSpr"),
						name);
					fprintf(fp, "\n");
				}
				if (err & 4) {
					warn_count++;
					fprintf(fp, messages_get("WarnAlphaSpr"),
						name);
					fprintf(fp, "\n");
				}
			}
		}
		if (throb_frames == -1) {
			fprintf(fp, messages_get("WarnNoThrob"));
			fprintf(fp, "\n");
		  
		} else {
			for (j = 0; j < throb_frames; j++) {
				sprintf(name, "throbber%i", j);
				err = ro_gui_dialog_test_sprite(name);
				if (err & 1) {
					warn_count++;
					fprintf(fp, messages_get("WarnMissThrob"),
						name);
					fprintf(fp, "\n");
				}
				if (err & 2) {
					warn_count++;
					fprintf(fp, messages_get("WarnHighSpr"),
						name);
					fprintf(fp, "\n");
				}
				if (err & 4) {
					warn_count++;
					fprintf(fp, messages_get("WarnAlphaSpr"),
						name);
					fprintf(fp, "\n");
				}
			}
		}
		
	} else {
		warn_count++;
		fprintf(fp, messages_get("WarnNoFile"));
		fprintf(fp, "\n");
	}
	if (warn_count > 0) {
		fprintf(fp, "\n");
		fprintf(fp, messages_get("CompleteErr"), warn_count);
		fprintf(fp, "\n");
	} else {
		fprintf(fp, messages_get("CompleteOK"));
		fprintf(fp, "\n");
	}
	
	fclose(fp);
	xosfile_set_type("<Wimp$Scrap>", 0xfff);
	xos_cli("Filer_Run <Wimp$Scrap>");
}

unsigned int ro_gui_dialog_test_sprite(const char *name) {
  	unsigned int result = 0;
  	int var_val;
	os_coord dimensions;
	os_mode mode;
	os_error *error;
	error = xosspriteop_read_sprite_info(osspriteop_USER_AREA,
			theme_sprites, (osspriteop_id)name,
			&dimensions.x, &dimensions.y, 0, &mode);
	if (error) return 1;
	if (((unsigned int)mode) & osspriteop_ALPHA_MASK) result |= 4;
	error = xos_read_mode_variable(mode, os_MODEVAR_LOG2_BPP,
			&var_val, 0);
	if (error) {
	 	LOG(("xos_read_mode_variable:  0x%x: %s",
				error->errnum, error->errmess));
	} else {
	  	if (var_val > 3) result |= 2;
	}
	return result;
}
