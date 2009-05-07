#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include "swis.h"

#include "tboxlibs/event.h"
#include "tboxlibs/gadgets.h"
#include "tboxlibs/quit.h"
#include "tboxlibs/toolbox.h"
#include "tboxlibs/wimp.h"
#include "tboxlibs/wimplib.h"

#include "encoding.h"
#include "ft.h"
#include "fm.h"
#include "glyph.h"
#include "glyphs.h"
#include "intmetrics.h"
#include "outlines.h"
#include "utils.h"

#define Convert_Font 0x101

int ttf2f_quit = 0;
MessagesFD messages_file;
ObjectId main_window;
int converting = 0;

/* Wimp Messages we're interested in */
static int messages [] ={
	Wimp_MDataLoad,
	Wimp_MQuit,
	0};

/* Toolbox events we're interested in */
static int tbcodes [] ={
	Convert_Font,
	Quit_Quit,
	Toolbox_Error,
	0};

static void ttf2f_init(int argc, char **argv);
static void ttf2f_exit(void);
static void register_toolbox_handlers(void);
static int toolbox_event_quit(int event_code, ToolboxEvent *event,
		IdBlock *id_block, void *handle);
static int toolbox_event_error(int event_code, ToolboxEvent *event,
		IdBlock *id_block, void *handle);
static int convert_font(int event_code, ToolboxEvent *event,
		IdBlock *id_block, void *handle);
static void register_message_handlers(void);
static int wimp_message_quit(WimpMessage *message,void *handle);
static int wimp_message_dataload(WimpMessage *message,void *handle);
static void progress_bar(int value);

int main(int argc, char **argv)
{
	ttf2f_init(argc, argv);

	while (!ttf2f_quit)
		ttf2f_poll(0);

	ttf2f_exit();

	return 0;
}


void ttf2f_init(int argc, char **argv)
{
	_kernel_oserror *error;
	IdBlock toolbox_block;
	char *stringset;

	ft_init();
	load_glyph_list();

	error = event_initialise(&toolbox_block);
	if (error) {
		fprintf(stderr, "event_initialise: 0x%x: %s\n",
			error->errnum, error->errmess);
		exit(1);
	}

	event_set_mask(Wimp_Poll_NullMask |
			Wimp_Poll_PointerLeavingWindowMask |
			Wimp_Poll_PointerEnteringWindowMask |
			Wimp_Poll_MouseClickMask |
			Wimp_Poll_KeyPressedMask |
			Wimp_Poll_LoseCaretMask |
			Wimp_Poll_GainCaretMask);

	register_toolbox_handlers();
	register_message_handlers();

	error = toolbox_initialise(0, 310, messages, tbcodes,
				"<TTF2F$Dir>", &messages_file,
				&toolbox_block, NULL, NULL, NULL);
	if (error) {
		fprintf(stderr, "toolbox_initialise: 0x%x: %s\n",
			error->errnum, error->errmess);
			exit(1);
	}

	error = toolbox_create_object(0, "main", &main_window);
	if (error) {
		fprintf(stderr, "toolbox_create_object: 0x%x: %s\n",
			error->errnum, error->errmess);
		exit(1);
	}

	stringset = getenv("Font$Path");
	if (stringset) {
		error = stringset_set_available(0, main_window, 7,
				stringset);
		if (error) {
			fprintf(stderr,
				"stringset_set_available: 0x%x: %s\n",
				error->errnum, error->errmess);
			exit(1);
		}
	}
}

void ttf2f_poll(int active)
{
	_kernel_oserror *error;
	int event;
	unsigned int mask;
	WimpPollBlock block;

	if (active || converting) {
		event_set_mask(0x3972);
		error = event_poll(&event, &block, 0);
	} else {
		event_get_mask(&mask);
		event_set_mask(mask | Wimp_Poll_NullMask);
		error = event_poll(&event, &block, 0);
	}
	if (error)
		fprintf(stderr, "event_poll: 0x%x: %s\n",
			error->errnum, error->errmess);
}

void ttf2f_exit(void)
{
	_kernel_oserror *error;

	ft_fini();
	destroy_glyphs();

	error = event_finalise();
	if (error)
		fprintf(stderr, "event_finalise: 0x%x: %s\n",
			error->errnum, error->errmess);
}

/**
 * Register event handlers with the toolbox
 */
void register_toolbox_handlers(void)
{
	_kernel_oserror *error;

	error = event_register_toolbox_handler(-1, Quit_Quit,
				toolbox_event_quit, 0);
	if (error)
		fprintf(stderr, "registering Quit_Quit: 0x%x: %s\n",
			error->errnum, error->errmess);

	error = event_register_toolbox_handler(-1, Toolbox_Error,
				toolbox_event_error, 0);
	if (error)
		fprintf(stderr, "registering Quit_Quit: 0x%x: %s\n",
			error->errnum, error->errmess);

	error = event_register_toolbox_handler(-1, Convert_Font,
				convert_font, 0);
	if (error)
		fprintf(stderr, "registering Convert_Font: 0x%x: %s\n",
				error->errnum, error->errmess);
}

/**
 * Handle quit events
 */
int toolbox_event_quit(int event_code, ToolboxEvent *event,
		IdBlock *id_block, void *handle)
{
	ttf2f_quit = 1;

	return 1;
}

/**
 * Handle toolbox errors
 */
int toolbox_event_error(int event_code, ToolboxEvent *event,
		IdBlock *id_block, void *handle)
{
	ToolboxErrorEvent *error = (ToolboxErrorEvent *)event;

	fprintf(stderr, "toolbox error: 0x%x: %s\n",
		error->errnum, error->errmess);

	return 1;
}

/**
 * Register message handlers
 */
void register_message_handlers(void)
{
	_kernel_oserror *error;

	error = event_register_message_handler(Wimp_MQuit, wimp_message_quit,
				0);
	if (error)
		fprintf(stderr, "registering Wimp_MQuit handler: 0x%x: %s\n",
			error->errnum, error->errmess);

	error = event_register_message_handler(Wimp_MDataLoad,
				wimp_message_dataload,
				0);
	if (error)
		fprintf(stderr,
			"registering Wimp_MDataLoad handler: 0x%x: %s\n",
			error->errnum, error->errmess);
}

/**
 * Handle quit messages
 */
int wimp_message_quit(WimpMessage *message,void *handle)
{
	ttf2f_quit = 1;

	return 1;
}

/**
 * Handle dataload messages
 */
int wimp_message_dataload(WimpMessage *message,void *handle)
{
	_kernel_oserror *error;
	WimpDataLoadMessage *dl = &message->data.data_load;

	error = displayfield_set_value(0, main_window, 0, dl->leaf_name);
	if (error) {
		fprintf(stderr, "displayfield_set_value: 0x%x: %s\n",
			error->errnum, error->errmess);
	}

	message->hdr.action_code = Wimp_MDataLoadAck;
	message->hdr.your_ref = message->hdr.my_ref;
	error = wimp_send_message(17, message, message->hdr.sender, 0, 0);
	if (error) {
		fprintf(stderr, "wimp_send_message: 0x%x: %s\n",
			error->errnum, error->errmess);
	}

	return 1;
}




int convert_font(int event_code, ToolboxEvent *event,
		IdBlock *id_block, void *handle)
{
	_kernel_oserror *error, erblock = { 123456, "Invalid Parameters" };
	char ifilename[256], ofilename[256], save_in[1024];
	char *token;
	int count = 0;
	int nglyphs, i;
	struct glyph *glyph_list, *g;
	struct font_metrics *metrics;

	if (converting)
		return 1;

	converting = 1;

	/* get input file */
	error = displayfield_get_value(0, main_window, 0, ifilename, 256, 0);
	if (error) {
		fprintf(stderr, "displayfield_get_value: 0x%x: %s\n",
			error->errnum, error->errmess);
	}

	/* read font name */
	error = writablefield_get_value(0, main_window, 3, ofilename, 256, 0);
	if (error) {
		fprintf(stderr, "writablefield_get_value: 0x%x: %s\n",
			error->errnum, error->errmess);
	}

	/* read save location */
	error = stringset_get_selected(0, main_window, 7, save_in, 1024, 0);
	if (error) {
		fprintf(stderr, "stringset_get_selected: 0x%x: %s\n",
			error->errnum, error->errmess);
	}

	/* sanity check */
	if (strcmp(save_in, "Save in") == 0 || strcmp(ofilename, "") == 0 ||
		strcmp(ifilename, "Filename") == 0) {
		wimp_report_error(&erblock, 0x5, "TTF2f");
		converting = 0;
		return 1;
	}

	/* create output directories */
	token = strtok(ofilename, ".");
	while (token) {
		if (count)
			strcat(save_in, ".");
		strcat(save_in, token);
		error = _swix(OS_File, _INR(0,1) | _IN(4), 8, save_in, 0);
		if (error) {
			fprintf(stderr, "os_file: 0x%x: %s\n",
				error->errnum, error->errmess);
			wimp_report_error(error, 0x5, "TTF2f");
			converting  = 0;
			return 1;
		}
		token = strtok(NULL, ".");
		count++;
	}

	/* re-read font name - previously corrupted by strtok */
	error = writablefield_get_value(0, main_window, 3, ofilename, 256, 0);
	if (error) {
		fprintf(stderr, "writablefield_get_value: 0x%x: %s\n",
			error->errnum, error->errmess);
	}

	if (open_font(ifilename)) {
		snprintf(erblock.errmess, 252, "Unknown font format");
		wimp_report_error(&erblock, 0x5, "TTF2f");
		converting = 0;
		return 1;
	}

	nglyphs = count_glyphs();

	fprintf(stderr, "%d x %d = %d\n", nglyphs, sizeof(struct glyph), nglyphs * sizeof(struct glyph));

	glyph_list = (struct glyph *)calloc(nglyphs, sizeof(struct glyph));
	if (!glyph_list) {
		fprintf(stderr, "malloc failed\n");
		snprintf(erblock.errmess, 252, "Insufficient memory");
		wimp_report_error(&erblock, 0x5, "TTF2f");
		converting = 0;
		return 1;
	}

	/* Initialise glyph list */
	for (i = 0; i != nglyphs; i++) {
		g = &glyph_list[i];

		g->code = -1;
	}

	/* create buffer for font metrics data */
	metrics = calloc(1, sizeof(struct font_metrics));
	if (!metrics) {
		free(glyph_list);
		close_font();
		fprintf(stderr, "malloc failed\n");
		snprintf(erblock.errmess, 252, "Insufficient memory");
		wimp_report_error(&erblock, 0x5, "TTF2f");
		converting = 0;
		return 1;
	}

	/* read global font metrics */
	if (fnmetrics(metrics)) {
		free(glyph_list);
		free(metrics);
		close_font();
		snprintf(erblock.errmess, 252, "Insufficient memory");
		wimp_report_error(&erblock, 0x5, "TTF2f");
		converting = 0;
		return 1;
	}

	/* map glyph ids to charcodes */
	if (glenc(glyph_list)) {
		free(glyph_list);
		free(metrics);
		close_font();
		snprintf(erblock.errmess, 252, "Unknown encoding");
		wimp_report_error(&erblock, 0x5, "TTF2f");
		converting = 0;
		return 1;
	}

	/* extract glyph names */
	if (glnames(glyph_list)) {
		free(glyph_list);
		free(metrics);
		close_font();
		snprintf(erblock.errmess, 252, "Insufficient memory");
		wimp_report_error(&erblock, 0x5, "TTF2f");
		converting = 0;
		return 1;
	}

	/* olive */
	slider_set_colour(0, main_window, 8, 13, 0);

	/* extract glyph metrics */
	glmetrics(glyph_list, progress_bar);

	/* red */
	slider_set_colour(0, main_window, 8, 11, 13);

	/* write intmetrics file */
	write_intmetrics(save_in, ofilename, glyph_list, nglyphs, metrics, progress_bar);

	/* blue */
	slider_set_colour(0, main_window, 8, 8, 11);

	/* write outlines file */
	write_outlines(save_in, ofilename, glyph_list, nglyphs, metrics, progress_bar);

	/* green */
	slider_set_colour(0, main_window, 8, 10, 8);

	/* write encoding file */
	write_encoding(save_in, ofilename, glyph_list, nglyphs, 0, progress_bar);

	/* reset slider */
	slider_set_colour(0, main_window, 8, 8, 0);
	slider_set_value(0, main_window, 8, 0);

	free(glyph_list);
	free(metrics);

	close_font();

	converting = 0;

	return 1;
}

void progress_bar(int value)
{
	slider_set_value(0, main_window, 8, value);
}

