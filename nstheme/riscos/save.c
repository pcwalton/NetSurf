/*
 * This file is part of NetSurf, http://netsurf.sourceforge.net/
 * Licensed under the GNU General Public License,
 *                http://www.opensource.org/licenses/gpl-license
 * Copyright 2004 James Bursa <bursa@users.sourceforge.net>
 */

/** \file
 * Save dialog and drag and drop saving (implementation).
 */

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "oslib/dragasprite.h"
#include "oslib/osgbpb.h"
#include "oslib/osfile.h"
#include "oslib/osfind.h"
#include "oslib/osspriteop.h"
#include "oslib/squash.h"
#include "oslib/wimp.h"
#include "nstheme/riscos/gui.h"
#include "nstheme/riscos/wimp.h"
#include "nstheme/utils/log.h"
#include "nstheme/utils/messages.h"
#include "nstheme/utils/utils.h"

#define SAVE_THEME 1
#define SAVE_SPRITES 2
static int gui_save_current_type = 0;
static char *reset_filename;
static unsigned int gui_save_filetype;

char *theme_filename = NULL;
char *sprite_filename = NULL;
static bool close_menu = true;

static void ro_gui_drag_icon(wimp_pointer *pointer);
static bool ro_gui_save(char *path);
static bool ro_gui_save_theme(char *path);
static bool ro_gui_save_sprites(char *path);

/**
 * Prepares the save box to reflect gui_save_type and a content, and
 * opens it.
 *
 * \param  save_type  type of save
 * \param  c          content to save
 * \param  sub_menu   open dialog as a sub menu, otherwise persistent
 * \param  x          x position, for sub_menu true only
 * \param  y          y position, for sub_menu true only
 * \param  parent     parent window for persistent box, for sub_menu false only
 */

void ro_gui_save_open(int save_type, int x, int y)
{
	char icon_buf[20];
	const char *icon = icon_buf;
	os_error *error;

	gui_save_current_type = save_type;
	if (save_type == SAVE_THEME) {
	  	if (theme_filename == NULL) {
	  		theme_filename = malloc(6);
	  		if (!theme_filename) {
	  		  	LOG(("No memory for malloc()"));
	  			warn_user("NoMemory", 0);
	  			return;
	  		}
	  		sprintf(theme_filename, "Theme");
	  	}
	 	gui_save_filetype = 0xffd;
	 	ro_gui_set_window_title(dialog_saveas, messages_get("SaveTitle"));
		ro_gui_set_icon_string(dialog_saveas, ICON_SAVE_PATH, theme_filename);
		reset_filename = theme_filename;
	} else {
	  	if (sprite_filename == NULL) {
	  		sprite_filename = malloc(8);
	  		if (!sprite_filename) {
	  		  	LOG(("No memory for malloc()"));
	  			warn_user("NoMemory", 0);
	  			return;
	  		}
	  		sprintf(sprite_filename, "Sprites");
	  	}
		gui_save_filetype = 0xff9;
	 	ro_gui_set_window_title(dialog_saveas, messages_get("ExportTitle"));
		ro_gui_set_icon_string(dialog_saveas, ICON_SAVE_PATH, sprite_filename);
		reset_filename = sprite_filename;
	}
	  

	/* icon */
	sprintf(icon_buf, "file_%.3x", gui_save_filetype);
	ro_gui_set_icon_string(dialog_saveas, ICON_SAVE_ICON, icon);

	/* open sub menu or persistent dialog */
	error = xwimp_create_sub_menu((wimp_menu *) dialog_saveas,
				x, y);
	if (error) {
		LOG(("xwimp_create_sub_menu: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("MenuError", error->errmess);
	}
}


/**
 * Handle clicks in the save dialog.
 */

void ro_gui_save_click(wimp_pointer *pointer)
{
	switch (pointer->i) {
	  	case ICON_SAVE_OK:
	  		close_menu = (pointer->buttons == wimp_CLICK_SELECT);
	  		ro_gui_save(ro_gui_get_icon_string(dialog_saveas, ICON_SAVE_PATH));
	  		if (pointer->buttons == wimp_CLICK_SELECT) {
	  		  	xwimp_create_menu((wimp_menu *)-1, 0, 0);
	  		  	ro_gui_dialog_close(pointer->w);
	  		}
	  		break;
	  	case ICON_SAVE_CANCEL:
	  		if (pointer->buttons == wimp_CLICK_SELECT) {
	  		  	xwimp_create_menu((wimp_menu *)-1, 0, 0);
	  		  	ro_gui_dialog_close(pointer->w);
	  		} else if (pointer->buttons == wimp_CLICK_ADJUST) {
	  			ro_gui_set_icon_string(dialog_saveas, ICON_SAVE_PATH, reset_filename);
	  		}
	  		break;
		case ICON_SAVE_ICON:
			if (pointer->buttons == wimp_DRAG_SELECT) {
				ro_gui_drag_icon(pointer);
			}
			break;
	}
}


/**
 * Start drag of icon under the pointer.
 */

void ro_gui_drag_icon(wimp_pointer *pointer)
{
	char *sprite;
	os_box box = { pointer->pos.x - 34, pointer->pos.y - 34,
			pointer->pos.x + 34, pointer->pos.y + 34 };
	os_error *error;

	if (pointer->i == -1)
		return;

	sprite = ro_gui_get_icon_string(pointer->w, pointer->i);

	error = xdragasprite_start(dragasprite_HPOS_CENTRE |
			dragasprite_VPOS_CENTRE |
			dragasprite_BOUND_POINTER |
			dragasprite_DROP_SHADOW,
			(osspriteop_area *) 1, sprite, &box, 0);
	if (error) {
		LOG(("xdragasprite_start: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("DragError", error->errmess);
	}
}


/**
 * Handle User_Drag_Box event for a drag from the save dialog.
 */

void ro_gui_save_drag_end(wimp_dragged *drag)
{
	char *name;
	char *dot;
	wimp_pointer pointer;
	wimp_message message;

	wimp_get_pointer_info(&pointer);

	name = ro_gui_get_icon_string(dialog_saveas, ICON_SAVE_PATH);
	dot = strrchr(name, '.');
	if (dot)
		name = dot + 1;

	message.your_ref = 0;
	message.action = message_DATA_SAVE;
	message.data.data_xfer.w = pointer.w;
	message.data.data_xfer.i = pointer.i;
	message.data.data_xfer.pos.x = pointer.pos.x;
	message.data.data_xfer.pos.y = pointer.pos.y;
	message.data.data_xfer.est_size = 1000;
	message.data.data_xfer.file_type = gui_save_filetype;
	strncpy(message.data.data_xfer.file_name, name, 212);
	message.data.data_xfer.file_name[211] = 0;
	message.size = 44 + ((strlen(message.data.data_xfer.file_name) + 4) &
			(~3u));

	wimp_send_message_to_window(wimp_USER_MESSAGE, &message,
			pointer.w, pointer.i);
}


/**
 * Handle Message_DataSaveAck for a drag from the save dialog.
 */

void ro_gui_save_datasave_ack(wimp_message *message)
{
	char *path = message->data.data_xfer.file_name;
	os_error *error;

	if (!ro_gui_save(path)) return;
	ro_gui_set_icon_string(dialog_saveas, ICON_SAVE_PATH, reset_filename);

	/*	Close the save window
	*/
	ro_gui_dialog_close(dialog_saveas);

	/* Ack successful save with message_DATA_LOAD */
	message->action = message_DATA_LOAD;
	message->your_ref = message->my_ref;
	error = xwimp_send_message_to_window(wimp_USER_MESSAGE, message,
			message->data.data_xfer.w, message->data.data_xfer.i, 0);
	if (error) {
		LOG(("xwimp_send_message_to_window: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("FileError", error->errmess);
	}

	if (close_menu) {
		error = xwimp_create_menu(wimp_CLOSE_MENU, 0, 0);
		if (error) {
			LOG(("xwimp_create_menu: 0x%x: %s",
					error->errnum, error->errmess));
			warn_user("MenuError", error->errmess);
		}
	}
	close_menu = true;
}

bool ro_gui_save(char *path) {
	switch (gui_save_current_type) {
		case SAVE_THEME:
			return ro_gui_save_theme(path);
		case SAVE_SPRITES:
			return ro_gui_save_sprites(path);
	}
	return false;
}

bool ro_gui_load_theme(char *path) {
  	os_error *error;
	struct theme_file_header *file_header;
	char *workspace;
	int workspace_size, file_size, output_left;
	fileswitch_object_type obj_type;
	squash_output_status status;
	os_fw file_handle;
	char *raw_data;
	char *compressed;
	char *decompressed;

	/*	Get memory for the header
	*/
	file_header = (struct theme_file_header *)calloc(1, 
				sizeof (struct theme_file_header));
	if (!file_header) {
	 	LOG(("No memory for calloc()"));
	 	warn_user("NoMemory", 0);
	}
	
	/*	Load the header
	*/
	error = xosfind_openinw(osfind_NO_PATH, path, 0, &file_handle);
	if (error) {
		free(file_header);
		LOG(("xosfind_openinw: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("FileError", error->errmess);
		return false;
	}
	if (file_handle == 0) {
		free(file_header);
		LOG(("File not present"));
		warn_user("FileError", error->errmess);
		return false;
	}
	error = xosgbpb_read_atw(file_handle, (char *)file_header,
			sizeof (struct theme_file_header),
			0, &output_left);
	xosfind_closew(file_handle);
	if (error) {
		free(file_header);
		LOG(("xosbgpb_read_atw: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("FileError", error->errmess);
		return false;
	}
	if (output_left > 0) {
		free(file_header);
		LOG(("Insufficient data"));
		warn_user("FileError", error->errmess);
		return false;
	}
	
	/*	Check the header is OK
	*/
	if ((file_header->magic_value != 0x4d54534e) ||
		(file_header->parser_version > 2)) return false;

	/*	Try to load the sprite file
	*/
	if (file_header->decompressed_sprite_size > 0) {
		error = xosfile_read_stamped_no_path(path,
				&obj_type, 0, 0, &file_size, 0, 0);
		if (error) {
			free(file_header);
			LOG(("xosfile_read_stamped_no_path: 0x%x: %s",
					error->errnum, error->errmess));
			warn_user("FileError", error->errmess);
			return false;
		}
		if (obj_type != fileswitch_IS_FILE) {
			free(file_header);
			return false;
		}
		raw_data = malloc(file_size);
		if (!raw_data) {
			free(file_header);
			LOG(("No memory for malloc()"));
			warn_user("NoMemory", 0);
			return false;
		}
		error = xosfile_load_stamped_no_path(path, (byte *)raw_data,
				0, 0, 0, 0, 0);
		if (error) {
			free(raw_data);
			free(file_header);
			LOG(("xosfile_load_stamped_no_path: 0x%x: %s",
					error->errnum, error->errmess));
			warn_user("FileError", error->errmess);
			return false;
		}
		
		/*	Decompress the data
		*/
		decompressed = malloc(file_header->decompressed_sprite_size);
		if (!decompressed) {
			free(raw_data);
			free(file_header);
			LOG(("No memory for malloc()"));
			warn_user("NoMemory", 0);
		}
		error = xsquash_decompress_return_sizes(-1, &workspace_size, 0);
		if (error) {
			free(decompressed);
			free(raw_data);
		  	free(file_header);
			LOG(("xsquash_decompress_return_sizes: 0x%x: %s",
					error->errnum, error->errmess));
			warn_user("MiscError", error->errmess);
			return false;
		}
		workspace = malloc(workspace_size);
		if (!workspace) {
			free(decompressed);
			free(raw_data);
		  	free(file_header);
			LOG(("No memory for malloc()"));
			warn_user("NoMemory", 0);
			return false;
		}
		compressed = raw_data + sizeof(struct theme_file_header);
		error = xsquash_decompress(0, /*squash_INPUT_ALL_PRESENT,*/
				workspace,
				(byte *)compressed, file_header->compressed_sprite_size,
				(byte *)decompressed, file_header->decompressed_sprite_size,
				&status, 0, 0, 0, 0);
		free(workspace);
		free(raw_data);
		if (error) {
			free(decompressed);
			LOG(("xsquash_decompress: 0x%x: %s",
					error->errnum, error->errmess));
			warn_user("MiscError", error->errmess);
			return false;
		}
		if (status != 0) {
		  	free(decompressed);
			return false;
		}
		if (theme_sprites) free(theme_sprites);
		theme_sprites = (osspriteop_area *)decompressed;		
	} else {
		if (theme_sprites) free(theme_sprites);
		theme_sprites = NULL;
	}
	
	/*	Set out values
	*/
	ro_gui_set_icon_string(dialog_main, ICON_MAIN_NAME, file_header->name);
	ro_gui_set_icon_string(dialog_main, ICON_MAIN_AUTHOR, file_header->author);
	ro_gui_set_icon_background_colour(dialog_main, ICON_MAIN_BROWSER_COLOUR,
			file_header->browser_bg);
	ro_gui_set_icon_background_colour(dialog_main, ICON_MAIN_HOTLIST_COLOUR,
			file_header->hotlist_bg);
	ro_gui_set_icon_background_colour(dialog_main, ICON_MAIN_STATUSBG_COLOUR,
			file_header->status_bg);
	ro_gui_set_icon_background_colour(dialog_main, ICON_MAIN_STATUSFG_COLOUR,
			file_header->status_fg);
	if (file_header->parser_version >= 2) {
		ro_gui_set_icon_selected_state(dialog_main, ICON_MAIN_THROBBER_LEFT,
				file_header->theme_flags & (1 << 0));
		ro_gui_set_icon_selected_state(dialog_main, ICON_MAIN_THROBBER_REDRAW,
				file_header->theme_flags & (1 << 1));
	} else {
		ro_gui_set_icon_selected_state(dialog_main, ICON_MAIN_THROBBER_LEFT,
				file_header->theme_flags);
		ro_gui_set_icon_selected_state(dialog_main, ICON_MAIN_THROBBER_REDRAW,
				true);
	}

	/*	Remember the filename for saving later on
	*/
	if (theme_filename) free(theme_filename);
	theme_filename = malloc(strlen(path) + 1);
	sprintf(theme_filename, "%s", path);
	if (sprite_filename) free(sprite_filename);
	sprite_filename = NULL;
	
	/*	Exit cleanly
	*/
	free(file_header);
	return true;
}

bool ro_gui_save_theme(char *path) {
  	os_error *error;
	struct theme_file_header *file_header;
	char *workspace;
	int workspace_size, output_size, input_left, output_left;
	squash_output_status status;

	/*	Get some memory
	*/
	unsigned int file_size = sizeof (struct theme_file_header);
	file_header = (struct theme_file_header *)calloc(file_size, 1);
	if (!file_header) {
	  	LOG(("No memory for calloc()"));
		warn_user("NoMemory", 0);
		return false;
	}

	/*	Complete the header
	*/
	file_header->magic_value = 0x4d54534e;
	file_header->parser_version = 2;
	strcpy(file_header->name, ro_gui_get_icon_string(dialog_main, ICON_MAIN_NAME));
	strcpy(file_header->author, ro_gui_get_icon_string(dialog_main, ICON_MAIN_AUTHOR));
	file_header->browser_bg = ro_gui_get_icon_background_colour(dialog_main,
			ICON_MAIN_BROWSER_COLOUR);
	file_header->hotlist_bg = ro_gui_get_icon_background_colour(dialog_main,
			ICON_MAIN_HOTLIST_COLOUR);
	file_header->status_bg = ro_gui_get_icon_background_colour(dialog_main,
			ICON_MAIN_STATUSBG_COLOUR);
	file_header->status_fg = ro_gui_get_icon_background_colour(dialog_main,
			ICON_MAIN_STATUSFG_COLOUR);
	if (ro_gui_get_icon_selected_state(dialog_main, ICON_MAIN_THROBBER_LEFT))
		file_header->theme_flags = (1 << 0);
	if (ro_gui_get_icon_selected_state(dialog_main, ICON_MAIN_THROBBER_REDRAW))
		file_header->theme_flags = (1 << 1);


	/*	Append the compressed sprites
	*/
	if (theme_sprites) {
		file_header->decompressed_sprite_size = theme_sprites->size;
		error = xsquash_compress_return_sizes(theme_sprites->size,
				&workspace_size, &output_size);
		if (error) {
		  	free(file_header);
			LOG(("xsquash_compress_return_sizes: 0x%x: %s",
					error->errnum, error->errmess));
			warn_user("MiscError", error->errmess);
			return false;
		}
		workspace = realloc(file_header,
				file_size + output_size);
		if (!workspace) {
		  	free(file_header);
			LOG(("No memory for realloc()"));
			warn_user("NoMemory", 0);
			return false;
		}
		file_header = (struct theme_file_header *)workspace;
		workspace = malloc(workspace_size);
		if (!workspace) {
		  	free(file_header);
			LOG(("No memory for malloc()"));
			warn_user("NoMemory", 0);
			return false;
		}
		error = xsquash_compress((squash_input_status)0,
				workspace, (byte *)theme_sprites,
				theme_sprites->size,
				(byte *)(file_header + 1), output_size,
				&status, 
				0, &input_left, 0, &output_left);
	  	free(workspace);
		if (error) {
		  	free(file_header);
			LOG(("xsquash_compress: 0x%x: %s",
					error->errnum, error->errmess));
			warn_user("MiscError", error->errmess);
			return false;
		}
		if ((input_left > 0) || (status != 0)) {
		  	free(file_header);
		  	LOG(("Failed to complete compression with %i bytes left and status %i",
		  		input_left, (int)status));
		  	warn_user("FileError", 0);
		  	return false;
		}
		file_header->compressed_sprite_size = output_size - output_left;
		file_size += output_size - output_left;
	}
	
	/*	Save the file
	*/
	error = xosfile_save_stamped(path, (bits)0xffd,
			(char *)file_header, ((char *)file_header) + file_size);
	free(file_header);
	if (error) {
		LOG(("xosfile_save_stamped: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("FileError", error->errmess);
		return false;
	}
	return false;
}

bool ro_gui_save_sprites(char *path) {
  	os_error *error;
	error = xosspriteop_save_sprite_file(osspriteop_USER_AREA,
		theme_sprites, path);
	if (error) {
		LOG(("xosfile_save_sprite_file: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("FileError", error->errmess);
		return false;
	}
	if (sprite_filename) {
		free(sprite_filename);
		sprite_filename = NULL;
	}
	sprite_filename = malloc(strlen(path) + 1);
	if (!sprite_filename) return true;
	sprintf(sprite_filename, "%s", path);
	return true;
}
