#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include <oslib/displayfield.h>
#include <oslib/osfile.h>
#include <oslib/quit.h>
#include <oslib/slider.h>
#include <oslib/stringset.h>
#include <oslib/writablefield.h>
#include <Event.h>

#include "encoding.h"
#include "ft.h"
#include "fm.h"
#include "glyph.h"
#include "glyphs.h"
#include "intmetrics.h"
#include "outlines.h"
#include "utils.h"

#define Convert_Font 0x101

static int ttf2f_quit;
static messagetrans_control_block messages_file;
static toolbox_o main_window;
static int converting;

typedef struct ttf2f_message_list {
	int first;
	int rest[];
} ttf2f_message_list;

typedef union message_list {
	ttf2f_message_list ttf2f;
	wimp_message_list wimp;
	toolbox_action_list toolbox;
} message_list;

#define PTR_WIMP_MESSAGE_LIST(l) ((wimp_message_list *) (message_list *) (l))
#define PTR_TOOLBOX_ACTION_LIST(l) ((toolbox_action_list *) (message_list *) (l))

/* Wimp Messages we're interested in */
static ttf2f_message_list messages = {
	message_DATA_LOAD,
	{
		message_QUIT,
		0
	}
};

/* Toolbox events we're interested in */
static ttf2f_message_list tbcodes = {
	Convert_Font,
	{
		action_ERROR,
		action_QUIT_QUIT,
		0
	}
};

static void ttf2f_init(int argc, char **argv);
static void ttf2f_exit(void);
static void register_toolbox_handlers(void);
static osbool toolbox_event_quit(bits event_code, toolbox_action *event,
		toolbox_block *id_block, void *handle);
static osbool toolbox_event_error(bits event_code, toolbox_action *event,
		toolbox_block *id_block, void *handle);
static osbool convert_font(bits event_code, toolbox_action *event,
		toolbox_block *id_block, void *handle);
static void register_message_handlers(void);
static int wimp_message_quit(wimp_message *message,void *handle);
static int wimp_message_dataload(wimp_message *message,void *handle);
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
	os_error *error;
	toolbox_block toolbox_block;
	char *stringset;
	ttf2f_result res;

	UNUSED(argc);
	UNUSED(argv);

	ft_init();

	res = glyph_load_list();
	if (res != TTF2F_RESULT_OK)
		exit(1);

	event_initialise(&toolbox_block);

	event_set_mask(wimp_MASK_NULL |
			wimp_MASK_LEAVING |
			wimp_MASK_ENTERING |
			wimp_QUEUE_MOUSE |
			wimp_QUEUE_KEY |
			wimp_MASK_LOSE |
			wimp_MASK_GAIN);

	register_toolbox_handlers();
	register_message_handlers();

	error = xtoolbox_initialise(0, 310,
				PTR_WIMP_MESSAGE_LIST(&messages), 
				PTR_TOOLBOX_ACTION_LIST(&tbcodes),
				"<TTF2F$Dir>", &messages_file,
				&toolbox_block, NULL, NULL, NULL);
	if (error) {
		fprintf(stderr, "toolbox_initialise: 0x%x: %s\n",
			error->errnum, error->errmess);
			exit(1);
	}

	error = xtoolbox_create_object(0, (toolbox_id) "main", &main_window);
	if (error) {
		fprintf(stderr, "toolbox_create_object: 0x%x: %s\n",
			error->errnum, error->errmess);
		exit(1);
	}

	stringset = getenv("Font$Path");
	if (stringset) {
		error = xstringset_set_available(0, main_window, 7,
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
	int event;
	unsigned int mask;
	wimp_block block;

	if (active || converting) {
		event_set_mask(0x3972);
		event_poll(&event, &block, 0);
	} else {
		event_get_mask(&mask);
		event_set_mask(mask | wimp_MASK_NULL);
		event_poll(&event, &block, 0);
	}
}

void ttf2f_exit(void)
{
	ft_fini();

	glyph_destroy_list();
}

/**
 * Register event handlers with the toolbox
 */
void register_toolbox_handlers(void)
{
	osbool success;

	success = event_register_toolbox_handler(event_ANY,
			action_QUIT_QUIT, toolbox_event_quit, NULL);
	if (success == FALSE)
		fprintf(stderr, "registering quit_QUIT failed\n");

	success = event_register_toolbox_handler(event_ANY, 
			action_ERROR, toolbox_event_error, NULL);
	if (success == FALSE)
		fprintf(stderr, "registering action_ERROR failed\n");

	success = event_register_toolbox_handler(event_ANY, 
			Convert_Font, convert_font, NULL);
	if (success == FALSE)
		fprintf(stderr, "registering Convert_Font failed\n");
}

/**
 * Handle quit events
 */
osbool toolbox_event_quit(bits event_code, toolbox_action *event,
		toolbox_block *id_block, void *handle)
{
	UNUSED(event_code);
	UNUSED(event);
	UNUSED(id_block);
	UNUSED(handle);

	ttf2f_quit = 1;

	return TRUE;
}

/**
 * Handle toolbox errors
 */
osbool toolbox_event_error(bits event_code, toolbox_action *event,
		toolbox_block *id_block, void *handle)
{
	toolbox_action_error_block *error = 
			(toolbox_action_error_block *) event;

	UNUSED(event_code);
	UNUSED(id_block);
	UNUSED(handle);

	fprintf(stderr, "toolbox error: 0x%x: %s\n",
		error->errnum, error->errmess);

	return TRUE;
}

/**
 * Register message handlers
 */
void register_message_handlers(void)
{
	osbool success;

	success = event_register_message_handler(message_QUIT, 
			wimp_message_quit, NULL);
	if (success == FALSE)
		fprintf(stderr, "registering message_QUIT handler failed\n");

	success = event_register_message_handler(message_DATA_LOAD,
			wimp_message_dataload, NULL);
	if (success == FALSE)
		fprintf(stderr,
			"registering message_DATA_LOAD handler failed\n");
}

/**
 * Handle quit messages
 */
osbool wimp_message_quit(wimp_message *message, void *handle)
{
	UNUSED(message);
	UNUSED(handle);

	ttf2f_quit = 1;

	return TRUE;
}

/**
 * Handle dataload messages
 */
osbool wimp_message_dataload(wimp_message *message, void *handle)
{
	os_error *error;
	wimp_message_data_xfer *dl = &message->data.data_xfer;

	UNUSED(handle);

	error = xdisplayfield_set_value(0, main_window, 0, dl->file_name);
	if (error != NULL) {
		fprintf(stderr, "displayfield_set_value: 0x%x: %s\n",
			error->errnum, error->errmess);
	}

	message->action = message_DATA_LOAD_ACK;
	message->your_ref = message->my_ref;

	error = xwimp_send_message(wimp_USER_MESSAGE, message, message->sender);
	if (error != NULL) {
		fprintf(stderr, "wimp_send_message: 0x%x: %s\n",
			error->errnum, error->errmess);
	}

	return TRUE;
}




osbool convert_font(bits event_code, toolbox_action *event,
		toolbox_block *id_block, void *handle)
{
	os_error *error, erblock = { 123456, "Invalid Parameters" };
	char ifilename[256], ofilename[256], save_in[1024];
	char *dot, *last_dot, *save;
	int fail;
	ttf2f_ctx ctx;
	ttf2f_result res;

	UNUSED(event_code);
	UNUSED(event);
	UNUSED(id_block);
	UNUSED(handle);

	if (converting)
		return TRUE;

	converting = 1;

	/* get input file */
	error = xdisplayfield_get_value(0, main_window, 0, ifilename, 256, 0);
	if (error) {
		fprintf(stderr, "displayfield_get_value: 0x%x: %s\n",
			error->errnum, error->errmess);
	}

	/* read font name */
	error = xwritablefield_get_value(0, main_window, 3, ofilename, 256, 0);
	if (error) {
		fprintf(stderr, "writablefield_get_value: 0x%x: %s\n",
			error->errnum, error->errmess);
	}

	/* read save location */
	error = xstringset_get_selected_string(0, main_window, 7, 
			save_in, 1024, 0);
	if (error) {
		fprintf(stderr, "stringset_get_selected: 0x%x: %s\n",
			error->errnum, error->errmess);
	}

	/* sanity check */
	if (strcmp(save_in, "Save in") == 0 || strcmp(ofilename, "") == 0 ||
		strcmp(ifilename, "Filename") == 0) {
		wimp_report_error(&erblock, 0x5, "TTF2f");
		converting = 0;
		return TRUE;
	}

	/* create output directories */
	last_dot = ofilename;
	save = save_in + strlen(save_in);

	for (dot = last_dot; *dot != '\0'; dot++) {
		if (*dot == '.') {
			error = xosfile_create_dir(save_in, 0);
			if (error) {
				fprintf(stderr, "os_file: 0x%x: %s\n",
					error->errnum, error->errmess);
				wimp_report_error(error, 0x5, "TTF2f");
				converting  = 0;
				return TRUE;
			}
		}

		*(save++) = *dot;
	}
	*save = '\0';

	error = xosfile_create_dir(save_in, 0);
	if (error) {
		fprintf(stderr, "os_file: 0x%x: %s\n",
			error->errnum, error->errmess);
		wimp_report_error(error, 0x5, "TTF2f");
		converting  = 0;
		return TRUE;
	}

	ctx.face = open_font(ifilename);
	if (ctx.face == NULL) {
		snprintf(erblock.errmess, 252, "Unknown font format");
		wimp_report_error(&erblock, 0x5, "TTF2f");
		converting = 0;
		return TRUE;
	}

	ctx.nglyphs = count_glyphs(&ctx);

	ctx.glyphs = calloc(ctx.nglyphs, sizeof(struct glyph));
	if (ctx.glyphs == NULL) {
		close_font(ctx.face);
		fprintf(stderr, "malloc failed\n");
		snprintf(erblock.errmess, 252, "Insufficient memory");
		wimp_report_error(&erblock, 0x5, "TTF2f");
		converting = 0;
		return TRUE;
	}

	/* Initialise glyph list */
	for (size_t i = 0; i != ctx.nglyphs; i++) {
		struct glyph *g = &ctx.glyphs[i];

		g->code = -1;
	}

	/* create buffer for font metrics data */
	ctx.metrics = calloc(1, sizeof(struct font_metrics));
	if (ctx.metrics == NULL) {
		free(ctx.glyphs);
		close_font(ctx.face);
		fprintf(stderr, "malloc failed\n");
		snprintf(erblock.errmess, 252, "Insufficient memory");
		wimp_report_error(&erblock, 0x5, "TTF2f");
		converting = 0;
		return TRUE;
	}

	/* read global font metrics */
	fail = fnmetrics(&ctx);
	if (fail) {
		free(ctx.metrics);
		free(ctx.glyphs);
		close_font(ctx.face);
		snprintf(erblock.errmess, 252, "Insufficient memory");
		wimp_report_error(&erblock, 0x5, "TTF2f");
		converting = 0;
		return TRUE;
	}

	/* map glyph ids to charcodes */
	fail = glenc(&ctx);
	if (fail) {
		free(ctx.metrics->name_copyright);
		free(ctx.metrics->name_full);
		free(ctx.metrics->name_version);
		free(ctx.metrics->name_ps);
		free(ctx.metrics);
		free(ctx.glyphs);
		close_font(ctx.face);
		snprintf(erblock.errmess, 252, "Unknown encoding");
		wimp_report_error(&erblock, 0x5, "TTF2f");
		converting = 0;
		return TRUE;
	}

	/* extract glyph names */
	fail = glnames(&ctx);
	if (fail) {
		free(ctx.metrics->name_copyright);
		free(ctx.metrics->name_full);
		free(ctx.metrics->name_version);
		free(ctx.metrics->name_ps);
		free(ctx.metrics);
		free(ctx.glyphs);
		close_font(ctx.face);
		snprintf(erblock.errmess, 252, "Insufficient memory");
		wimp_report_error(&erblock, 0x5, "TTF2f");
		converting = 0;
		return TRUE;
	}

	/* olive */
	slider_set_colour(0, main_window, 8, 13, 0);

	/* extract glyph metrics */
	glmetrics(&ctx, progress_bar);

	/* red */
	slider_set_colour(0, main_window, 8, 11, 13);

	/* write intmetrics file */
	res = intmetrics_write(save_in, ofilename, &ctx, progress_bar);
	if (res != TTF2F_RESULT_OK) {
		free(ctx.metrics->name_copyright);
		free(ctx.metrics->name_full);
		free(ctx.metrics->name_version);
		free(ctx.metrics->name_ps);
		free(ctx.metrics);
		free(ctx.glyphs);
		close_font(ctx.face);
		snprintf(erblock.errmess, 252, "Insufficient memory");
		wimp_report_error(&erblock, 0x5, "TTF2f");
		converting = 0;
		return TRUE;
	}

	/* blue */
	slider_set_colour(0, main_window, 8, 8, 11);

	/* write outlines file */
	res = outlines_write(save_in, ofilename, &ctx, progress_bar);
	if (res != TTF2F_RESULT_OK) {
		free(ctx.metrics->name_copyright);
		free(ctx.metrics->name_full);
		free(ctx.metrics->name_version);
		free(ctx.metrics->name_ps);
		free(ctx.metrics);
		free(ctx.glyphs);
		close_font(ctx.face);
		snprintf(erblock.errmess, 252, "Insufficient memory");
		wimp_report_error(&erblock, 0x5, "TTF2f");
		converting = 0;
		return TRUE;
	}

	/* green */
	slider_set_colour(0, main_window, 8, 10, 8);

	/* write encoding file */
	res = encoding_write(save_in, ofilename, &ctx, 
			ENCODING_TYPE_NORMAL, progress_bar);
	if (res != TTF2F_RESULT_OK) {
		free(ctx.metrics->name_copyright);
		free(ctx.metrics->name_full);
		free(ctx.metrics->name_version);
		free(ctx.metrics->name_ps);
		free(ctx.metrics);
		free(ctx.glyphs);
		close_font(ctx.face);
		snprintf(erblock.errmess, 252, "Insufficient memory");
		wimp_report_error(&erblock, 0x5, "TTF2f");
		converting = 0;
		return TRUE;
	}

	/* reset slider */
	slider_set_colour(0, main_window, 8, 8, 0);
	slider_set_value(0, main_window, 8, 0);

	free(ctx.metrics->name_copyright);
	free(ctx.metrics->name_full);
	free(ctx.metrics->name_version);
	free(ctx.metrics->name_ps);
	free(ctx.metrics);
	free(ctx.glyphs);

	close_font(ctx.face);

	converting = 0;

	return TRUE;
}

void progress_bar(int value)
{
	slider_set_value(0, main_window, 8, value);
}

