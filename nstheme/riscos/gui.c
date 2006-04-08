/*
 * This file is part of NetSurf, http://netsurf.sourceforge.net/
 * Licensed under the GNU General Public License,
 *                http://www.opensource.org/licenses/gpl-license
 * Copyright 2003 Phil Mellor <monkeyson@users.sourceforge.net>
 * Copyright 2004 James Bursa <bursa@users.sourceforge.net>
 * Copyright 2003 John M Bell <jmb202@ecs.soton.ac.uk>
 * Copyright 2004 Richard Wilson <not_ginger_matt@users.sourceforge.net>
 */

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unixlib/features.h>
#include <unixlib/local.h>
#include "oslib/font.h"
#include "oslib/help.h"
#include "oslib/hourglass.h"
#include "oslib/inetsuite.h"
#include "oslib/os.h"
#include "oslib/osbyte.h"
#include "oslib/osfile.h"
#include "oslib/osfscontrol.h"
#include "oslib/osspriteop.h"
#include "oslib/wimp.h"
#include "oslib/wimpspriteop.h"
#include "oslib/uri.h"
#include "nstheme/desktop/gui.h"
#include "nstheme/desktop/nstheme.h"
#include "nstheme/desktop/options.h"
#include "nstheme/riscos/gui.h"
#include "nstheme/riscos/help.h"
#include "nstheme/riscos/options.h"
#include "nstheme/riscos/wimp.h"
#include "nstheme/utils/log.h"
#include "nstheme/utils/messages.h"
#include "nstheme/utils/utils.h"

const char *__dynamic_da_name = "NSTheme";	/**< For UnixLib. */
int __feature_imagefs_is_file = 1;              /**< For UnixLib. */
/* default filename handling */
int __riscosify_control = __RISCOSIFY_NO_SUFFIX |
			__RISCOSIFY_NO_REVERSE_SUFFIX;

char *NSTHEME_DIR;

wimp_t task_handle;	/**< RISC OS wimp task handle. */
static clock_t gui_last_poll;	/**< Time of last wimp_poll. */

/** Accepted wimp user messages. */
static wimp_MESSAGE_LIST(33) task_messages = { {
  	message_HELP_REQUEST,
	message_DATA_SAVE,
	message_DATA_SAVE_ACK,
	message_DATA_LOAD,
	message_DATA_LOAD_ACK,
	message_MENU_WARNING,
	0
} };
struct ro_gui_poll_block {
	wimp_event_no event;
	wimp_block *block;
	struct ro_gui_poll_block *next;
};
struct ro_gui_poll_block *ro_gui_poll_queued_blocks = 0;


static void ro_gui_choose_language(void);
static void ro_gui_icon_bar_create(void);
static void ro_gui_handle_event(wimp_event_no event, wimp_block *block);
static void ro_gui_open_window_request(wimp_open *open);
static void ro_gui_mouse_click(wimp_pointer *pointer);
static void ro_gui_icon_bar_click(wimp_pointer *pointer);
static void ro_gui_keypress(wimp_key *key);
static void ro_gui_user_message(wimp_event_no event, wimp_message *message);
static void ro_msg_dataload(wimp_message *block);


/**
 * Initialise the gui (RISC OS specific part).
 */

void gui_init(void) {
	char path[40];
	os_error *error;
	int length;

	xhourglass_start(1);

	options_read("Choices:NSTheme.Choices");
	ro_gui_choose_language();

	NSTHEME_DIR = getenv("NSTheme$Dir");
	if ((length = snprintf(path, sizeof(path),
			"<NSTheme$Dir>.Resources.%s.Messages",
			option_language)) < 0 || length >= (int)sizeof(path))
		die("Failed to locate Messages resource.");
	messages_load(path);

        /* Totally pedantic, but base the taskname on the build options.
        */
	error = xwimp_initialise(wimp_VERSION_RO38, "NSTheme",
			(const wimp_message_list *) &task_messages, 0,
			&task_handle);
	if (error) {
		LOG(("xwimp_initialise failed: 0x%x: %s",
				error->errnum, error->errmess));
		die(error->errmess);
	}

	/*	Open the templates
	*/
	if ((length = snprintf(path, sizeof(path),
			"<NSTheme$Dir>.Resources.%s.Templates",
			option_language)) < 0 || length >= (int)sizeof(path))
		die("Failed to locate Templates resource.");
	error = xwimp_open_template(path);
	if (error) {
		LOG(("xwimp_open_template failed: 0x%x: %s",
				error->errnum, error->errmess));
		die(error->errmess);
	}
	ro_gui_dialog_init();
	ro_gui_menus_init();
	xwimp_close_template();

	ro_gui_icon_bar_create();
}

/**
 * Determine the language to use.
 *
 * RISC OS has no standard way of determining which language the user prefers.
 * We have to guess from the 'Country' setting.
 */

void ro_gui_choose_language(void)
{
	char path[40];
	const char *lang;
	int country;
	os_error *error;

	/* if option_language exists and is valid, use that */
	if (option_language) {
		if (2 < strlen(option_language))
			option_language[2] = 0;
		sprintf(path, "<NSTheme$Dir>.Resources.%s", option_language);
		if (is_dir(path)) return;
		free(option_language);
		option_language = 0;
	}

	/* choose a language from the configured country number */
	error = xosbyte_read(osbyte_VAR_COUNTRY_NUMBER, &country);
	if (error) {
		LOG(("xosbyte_read failed: 0x%x: %s",
				error->errnum, error->errmess));
		country = 1;
	}
	switch (country) {
		case 6: /* France */
		case 18: /* Canada2 (French Canada?) */
			lang = "fr";
			break;
		default:
			lang = "en";
			break;
	}
	sprintf(path, "<NSTheme$Dir>.Resources.%s", lang);
	if (is_dir(path))
		option_language = strdup(lang);
	else
		option_language = strdup("en");
	assert(option_language);
}


/**
 * Create an iconbar icon.
 */

void ro_gui_icon_bar_create(void)
{
	wimp_icon_create icon = {
		wimp_ICON_BAR_RIGHT,
		{ { 0, 0, 68, 68 },
		wimp_ICON_SPRITE | wimp_ICON_HCENTRED | wimp_ICON_VCENTRED |
				(wimp_BUTTON_CLICK << wimp_ICON_BUTTON_TYPE_SHIFT),
		{ "!nstheme" } } };
	wimp_create_icon(&icon);
}



/**
 * Close down the gui (RISC OS).
 */

void gui_exit(void) {
	xosfile_create_dir("<Choices$Write>.NSTheme", 0);
	options_write("<Choices$Write>.NSTheme.Choices");
	xwimp_close_down(task_handle);
	xhourglass_off();
}


/**
 * Poll the OS for events (RISC OS).
 *
 * \param active return as soon as possible
 */

void gui_poll(void)
{
	wimp_event_no event;
	wimp_block block;
	const wimp_poll_flags mask = wimp_MASK_LOSE | wimp_MASK_GAIN;

	/* Process queued events. */
	while (ro_gui_poll_queued_blocks) {
		struct ro_gui_poll_block *next;
		ro_gui_handle_event(ro_gui_poll_queued_blocks->event,
				ro_gui_poll_queued_blocks->block);
		next = ro_gui_poll_queued_blocks->next;
		free(ro_gui_poll_queued_blocks->block);
		free(ro_gui_poll_queued_blocks);
		ro_gui_poll_queued_blocks = next;
	}

	/* Poll wimp. */
	xhourglass_off();
	event = wimp_poll(wimp_MASK_NULL | mask, &block, 0);
	xhourglass_on();
	gui_last_poll = clock();
	ro_gui_handle_event(event, &block);
}


/**
 * Process a Wimp_Poll event.
 *
 * \param event wimp event number
 * \param block parameter block
 */

void ro_gui_handle_event(wimp_event_no event, wimp_block *block)
{
	switch (event) {
		case wimp_OPEN_WINDOW_REQUEST:
			ro_gui_open_window_request(&block->open);
			break;

		case wimp_CLOSE_WINDOW_REQUEST:
			ro_gui_dialog_close(block->close.w);
			break;

		case wimp_MOUSE_CLICK:
			ro_gui_mouse_click(&block->pointer);
			break;

		case wimp_USER_DRAG_BOX:
			ro_gui_save_drag_end(&(block->dragged));
			break;

		case wimp_KEY_PRESSED:
			ro_gui_keypress(&(block->key));
			break;

		case wimp_MENU_SELECTION:
			ro_gui_menu_selection(&(block->selection));
			break;

		case wimp_USER_MESSAGE:
		case wimp_USER_MESSAGE_RECORDED:
		case wimp_USER_MESSAGE_ACKNOWLEDGE:
			ro_gui_user_message(event, &(block->message));
			break;
	}
}


/**
 * Handle Open_Window_Request events.
 */

void ro_gui_open_window_request(wimp_open *open) {
	os_error *error;
	error = xwimp_open_window(open);
	if (error) {
		LOG(("xwimp_open_window: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("WimpError", error->errmess);
		return;
	}
}


/**
 * Handle Mouse_Click events.
 */
void ro_gui_mouse_click(wimp_pointer *pointer) {
	if (pointer->w == wimp_ICON_BAR) {
		ro_gui_icon_bar_click(pointer);
	} else if (pointer->w == dialog_saveas) {
		ro_gui_save_click(pointer);
	} else {
		ro_gui_dialog_click(pointer);
        }
}

/**
 * Handle Mouse_Click events on the iconbar icon.
 */

void ro_gui_icon_bar_click(wimp_pointer *pointer)
{
	if (pointer->buttons == wimp_CLICK_MENU) {
		ro_gui_create_menu(iconbar_menu, pointer->pos.x,
				   96 + iconbar_menu_height);
	} else if (pointer->buttons == wimp_CLICK_SELECT) {
	  	ro_gui_dialog_prepare_main();
		ro_gui_open_window_centre(dialog_main);
		ro_gui_set_caret_first(dialog_main);
	}
}


/**
 * Handle Key_Pressed events.
 */

void ro_gui_keypress(wimp_key *key) {
	bool handled = false;
	os_error *error;

	if (key->c == wimp_KEY_F1) {
		xos_cli("Filer_Run <NSTheme$Dir>.!Help");
		return;
	}

	if (key->w == dialog_saveas) {
//		handled = ro_gui_saveas_keypress(key->c);
        } else {
		handled = ro_gui_dialog_keypress(key);
	}

	if (!handled) {
		error = xwimp_process_key(key->c);
		if (error) {
			LOG(("xwimp_process_key: 0x%x: %s",
					error->errnum, error->errmess));
			warn_user("WimpError", error->errmess);
		}
	}
}


/**
 * Handle the three User_Message events.
 */

void ro_gui_user_message(wimp_event_no event, wimp_message *message)
{
	switch (message->action) {
		case message_HELP_REQUEST:
			ro_gui_interactive_help_request(message);
			break;

		case message_DATA_SAVE_ACK:
			ro_gui_save_datasave_ack(message);
			break;

		case message_DATA_LOAD:
			if (event != wimp_USER_MESSAGE_ACKNOWLEDGE)
				ro_msg_dataload(message);
			break;

		case message_DATA_LOAD_ACK:
			break;

		case message_MENU_WARNING:
			ro_gui_menu_warning((wimp_message_menu_warning *)
					&message->data);
			break;

		case message_QUIT:
			application_quit = true;
			break;
	}
}


/**
 * Handle Message_DataLoad (file dragged in).
 */

void ro_msg_dataload(wimp_message *message) {
  	os_error *error;
  	osspriteop_area *area;
	bool success = false;
	char *filename = message->data.data_xfer.file_name;
	int file_type = message->data.data_xfer.file_type;
	if ((file_type == 0xff9) && (message->data.data_xfer.w == dialog_main)) {
		area = ro_gui_load_sprite_file(filename);
		if (area) {
			success = true;
			if (theme_sprites) free(theme_sprites);
			theme_sprites = area;
		  	ro_gui_dialog_prepare_main();
		  	if (sprite_filename) {
		  		free(sprite_filename);
		  		sprite_filename = NULL;
		  	}
		} else {
			warn_user("WarnBadSpr", 0);
		}
	} else {
		success = ro_gui_load_theme(filename);
		if (success) {
		  	ro_gui_dialog_prepare_main();
			ro_gui_open_window_centre(dialog_main);
			ro_gui_set_caret_first(dialog_main);
		} else {
			warn_user("WarnInvalid", 0);
		}
	}

	if (!success) return;
	message->action = message_DATA_LOAD_ACK;
	message->your_ref = message->my_ref;
	error = xwimp_send_message(wimp_USER_MESSAGE, message, message->sender);
	if (error) {
		LOG(("xwimp_send_message: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("WimpError", error->errmess);
		return;
	}
}


/**
 * Find screen size in OS units.
 */

void ro_gui_screen_size(int *width, int *height)
{
	int xeig_factor, yeig_factor, xwind_limit, ywind_limit;

	os_read_mode_variable(os_CURRENT_MODE, os_MODEVAR_XEIG_FACTOR, &xeig_factor);
	os_read_mode_variable(os_CURRENT_MODE, os_MODEVAR_YEIG_FACTOR, &yeig_factor);
	os_read_mode_variable(os_CURRENT_MODE, os_MODEVAR_XWIND_LIMIT, &xwind_limit);
	os_read_mode_variable(os_CURRENT_MODE, os_MODEVAR_YWIND_LIMIT, &ywind_limit);
	*width = (xwind_limit + 1) << xeig_factor;
	*height = (ywind_limit + 1) << yeig_factor;
}


/**
 * Display a warning for a serious problem (eg memory exhaustion).
 *
 * \param  warning  message key for warning message
 * \param  detail   additional message, or 0
 */

void warn_user(const char *warning, const char *detail)
{
	static char warn_buffer[300];

	LOG(("%s %s", warning, detail));
	snprintf(warn_buffer, sizeof warn_buffer, "%s %s",
			messages_get(warning),
			detail ? detail : "");
	warn_buffer[sizeof warn_buffer - 1] = 0;
	ro_gui_set_icon_string(dialog_warning, ICON_WARNING_MESSAGE,
			warn_buffer);
	xwimp_set_icon_state(dialog_warning, ICON_WARNING_HELP,
			wimp_ICON_DELETED, wimp_ICON_DELETED);
	ro_gui_dialog_open(dialog_warning);
	xos_bell();
}


/**
 * Display an error and exit.
 *
 * Should only be used during initialisation.
 */

void die(const char *error)
{
	os_error warn_error;

	warn_error.errnum = 1; /* \todo: reasonable ? */
	strncpy(warn_error.errmess, messages_get(error),
			sizeof(warn_error.errmess)-1);
	warn_error.errmess[sizeof(warn_error.errmess)-1] = '\0';
	xwimp_report_error_by_category(&warn_error,
			wimp_ERROR_BOX_OK_ICON |
			wimp_ERROR_BOX_GIVEN_CATEGORY |
			wimp_ERROR_BOX_CATEGORY_ERROR <<
				wimp_ERROR_BOX_CATEGORY_SHIFT,
			"NSTheme", "!nstheme",
			(osspriteop_area *) 1, 0, 0);
	exit(EXIT_FAILURE);
}
