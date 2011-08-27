/*
 * Copyright 2006 Richard Wilson <info@tinct.net>
 * Copyright 2008 Sean Fox <dyntryx@gmail.com>
 *
 * This file is part of NetSurf, http://www.netsurf-browser.org/
 *
 * NetSurf is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * NetSurf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/** \file
 * Content for image/bmp (implementation)
 */

#include "utils/config.h"
#ifdef WITH_BMP

#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <libnsbmp.h>
#include "utils/config.h"
#include "content/content_protected.h"
#include "content/hlcache.h"
#include "desktop/plotters.h"
#include "image/bitmap.h"
#include "image/bmp.h"
#include "utils/log.h"
#include "utils/messages.h"
#include "utils/talloc.h"
#include "utils/utils.h"

typedef struct nsbmp_content {
	struct content base;

	bmp_image *bmp;	/** BMP image data */
} nsbmp_content;

static nserror nsbmp_create_bmp_data(nsbmp_content *bmp)
{	
	union content_msg_data msg_data;

	bmp->bmp = calloc(sizeof(struct bmp_image), 1);
	if (bmp->bmp == NULL) {
		msg_data.error = messages_get("NoMemory");
		content_broadcast(&bmp->base, CONTENT_MSG_ERROR, msg_data);
		return NSERROR_NOMEM;
	}

	bmp_create(bmp->bmp, &bmp_bitmap_callbacks);

	return NSERROR_OK;
}


static nserror nsbmp_create(const content_handler *handler,
		lwc_string *imime_type, const struct http_parameter *params,
		llcache_handle *llcache, const char *fallback_charset,
		bool quirks, struct content **c)
{
	nsbmp_content *bmp;
	nserror error;

	bmp = talloc_zero(0, nsbmp_content);
	if (bmp == NULL)
		return NSERROR_NOMEM;

	error = content__init(&bmp->base, handler, imime_type, params,
			llcache, fallback_charset, quirks);
	if (error != NSERROR_OK) {
		talloc_free(bmp);
		return error;
	}

	error = nsbmp_create_bmp_data(bmp);
	if (error != NSERROR_OK) {
		talloc_free(bmp);
		return error;
	}

	*c = (struct content *) bmp;

	return NSERROR_OK;
}

/**
 * Callback for libnsbmp; forwards the call to bitmap_create()
 *
 * \param  width   width of image in pixels
 * \param  height  width of image in pixels
 * \param  state   a flag word indicating the initial state
 * \return an opaque struct bitmap, or NULL on memory exhaustion
 */
static void *nsbmp_bitmap_create(int width, int height, unsigned int bmp_state)
{
	unsigned int bitmap_state = BITMAP_NEW;

	/* set bitmap state based on bmp state */
	bitmap_state |= (bmp_state & BMP_OPAQUE) ? BITMAP_OPAQUE : 0;
	bitmap_state |= (bmp_state & BMP_CLEAR_MEMORY) ?
			BITMAP_CLEAR_MEMORY : 0;

	/* return the created bitmap */
	return bitmap_create(width, height, bitmap_state);
}

/* The Bitmap callbacks function table;
 * necessary for interaction with nsbmplib.
 */
bmp_bitmap_callback_vt bmp_bitmap_callbacks = {
	.bitmap_create = nsbmp_bitmap_create,
	.bitmap_destroy = bitmap_destroy,
	.bitmap_set_suspendable = bitmap_set_suspendable,
	.bitmap_get_buffer = bitmap_get_buffer,
	.bitmap_get_bpp = bitmap_get_bpp
};

static bool nsbmp_convert(struct content *c)
{
	nsbmp_content *bmp = (nsbmp_content *) c;
	bmp_result res;
	union content_msg_data msg_data;
	uint32_t swidth;
	const char *data;
	unsigned long size;
	char title[100];

	/* set the bmp data */
	data = content__get_source_data(c, &size);

	/* analyse the BMP */
	res = bmp_analyse(bmp->bmp, size, (unsigned char *) data);
	switch (res) {
		case BMP_OK:
			break;
		case BMP_INSUFFICIENT_MEMORY:
			msg_data.error = messages_get("NoMemory");
			content_broadcast(c, CONTENT_MSG_ERROR, msg_data);
			return false;
		case BMP_INSUFFICIENT_DATA:
		case BMP_DATA_ERROR:
			msg_data.error = messages_get("BadBMP");
			content_broadcast(c, CONTENT_MSG_ERROR, msg_data);
			return false;
	}

	/* Store our content width and description */
	c->width = bmp->bmp->width;
	c->height = bmp->bmp->height;
	LOG(("BMP      width %u       height %u", c->width, c->height));
	snprintf(title, sizeof(title), messages_get("BMPTitle"),
			c->width, c->height, size);
	content__set_title(c, title);
	swidth = bmp->bmp->bitmap_callbacks.bitmap_get_bpp(bmp->bmp->bitmap) * 
			bmp->bmp->width;
	c->size += (swidth * bmp->bmp->height) + 16 + 44;

	/* exit as a success */
	c->bitmap = bmp->bmp->bitmap;
	bitmap_modified(c->bitmap);

	content_set_ready(c);
	content_set_done(c);

	/* Done: update status bar */
	content_set_status(c, "");
	return true;
}

static bool nsbmp_redraw(struct content *c, struct content_redraw_data *data,
		const struct rect *clip, const struct redraw_context *ctx)
{
	nsbmp_content *bmp = (nsbmp_content *) c;
	bitmap_flags_t flags = BITMAPF_NONE;

	if (bmp->bmp->decoded == false)
		if (bmp_decode(bmp->bmp) != BMP_OK)
			return false;

	c->bitmap = bmp->bmp->bitmap;

	if (data->repeat_x)
		flags |= BITMAPF_REPEAT_X;
	if (data->repeat_y)
		flags |= BITMAPF_REPEAT_Y;

	return ctx->plot->bitmap(data->x, data->y, data->width, data->height,
			c->bitmap, data->background_colour, flags);
}


static void nsbmp_destroy(struct content *c)
{
	nsbmp_content *bmp = (nsbmp_content *) c;

	bmp_finalise(bmp->bmp);
	free(bmp->bmp);
}


static nserror nsbmp_clone(const struct content *old, struct content **newc)
{
	nsbmp_content *new_bmp;
	nserror error;

	new_bmp = talloc_zero(0, nsbmp_content);
	if (new_bmp == NULL)
		return NSERROR_NOMEM;

	error = content__clone(old, &new_bmp->base);
	if (error != NSERROR_OK) {
		content_destroy(&new_bmp->base);
		return error;
	}

	/* We "clone" the old content by replaying creation and conversion */
	error = nsbmp_create_bmp_data(new_bmp);
	if (error != NSERROR_OK) {
		content_destroy(&new_bmp->base);
		return error;
	}

	if (old->status == CONTENT_STATUS_READY || 
			old->status == CONTENT_STATUS_DONE) {
		if (nsbmp_convert(&new_bmp->base) == false) {
			content_destroy(&new_bmp->base);
			return NSERROR_CLONE_FAILED;
		}
	}

	*newc = (struct content *) new_bmp;

	return NSERROR_OK;
}

static content_type nsbmp_content_type(lwc_string *mime_type)
{
	return CONTENT_IMAGE;
}


static const content_handler nsbmp_content_handler = {
	.create = nsbmp_create,
	.data_complete = nsbmp_convert,
	.destroy = nsbmp_destroy,
	.redraw = nsbmp_redraw,
	.clone = nsbmp_clone,
	.type = nsbmp_content_type,
	.no_share = false,
};

static const char *nsbmp_types[] = {
	"application/bmp",
	"application/preview",
	"application/x-bmp",
	"application/x-win-bitmap",
	"image/bmp",
	"image/ms-bmp",
	"image/x-bitmap",
	"image/x-bmp",
	"image/x-ms-bmp",
	"image/x-win-bitmap",
	"image/x-windows-bmp",
	"image/x-xbitmap"
};

CONTENT_FACTORY_REGISTER_TYPES(nsbmp, nsbmp_types, nsbmp_content_handler);

#endif
