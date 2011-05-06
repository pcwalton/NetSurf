/*
 * Copyright 2003 John M Bell <jmb202@ecs.soton.ac.uk>
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
 * Content for image/x-riscos-sprite (RISC OS implementation).
 *
 * No conversion is necessary: we can render RISC OS sprites directly under
 * RISC OS.
 */

#include <string.h>
#include <stdlib.h>
#include "oslib/osspriteop.h"
#include "utils/config.h"
#include "content/content_protected.h"
#include "desktop/plotters.h"
#include "riscos/gui.h"
#include "riscos/image.h"
#include "riscos/sprite.h"
#include "utils/config.h"
#include "utils/log.h"
#include "utils/messages.h"
#include "utils/talloc.h"
#include "utils/utils.h"

#ifdef WITH_SPRITE

typedef struct sprite_content {
	struct content base;

	void *data;
} sprite_content;

static nserror sprite_create(const content_handler *handler,
		lwc_string *imime_type, const http_parameter *params,
		llcache_handle *llcache, const char *fallback_charset,
		bool quirks, struct content **c);
static bool sprite_convert(struct content *c);
static void sprite_destroy(struct content *c);
static bool sprite_redraw(struct content *c, int x, int y,
		int width, int height, const struct rect *clip,
		float scale, colour background_colour);
static nserror sprite_clone(const struct content *old, struct content **newc);
static content_type sprite_content_type(lwc_string *mime_type);

static const content_handler sprite_content_handler = {
	sprite_create,
	NULL,
	sprite_convert,
	NULL,
	sprite_destroy,
	NULL,
	NULL,
	NULL,
	sprite_redraw,
	NULL,
	NULL,
	NULL,
	sprite_clone,
	NULL,
	sprite_content_type,
	false
};

static const char *sprite_types[] = {
	"image/x-riscos-sprite"
};

static lwc_string *sprite_mime_types[NOF_ELEMENTS(sprite_types)];

nserror sprite_init(void)
{
	uint32_t i;
	lwc_error lerror;
	nserror error;

	for (i = 0; i < NOF_ELEMENTS(sprite_mime_types); i++) {
		lerror = lwc_intern_string(sprite_types[i],
				strlen(sprite_types[i]),
				&sprite_mime_types[i]);
		if (lerror != lwc_error_ok) {
			error = NSERROR_NOMEM;
			goto error;
		}

		error = content_factory_register_handler(sprite_mime_types[i],
				&sprite_content_handler);
		if (error != NSERROR_OK)
			goto error;
	}

	return NSERROR_OK;

error:
	sprite_fini();

	return error;
}

void sprite_fini(void)
{
	uint32_t i;

	for (i = 0; i < NOF_ELEMENTS(sprite_mime_types); i++) {
		if (sprite_mime_types[i] != NULL)
			lwc_string_unref(sprite_mime_types[i]);
	}
}

nserror sprite_create(const content_handler *handler,
		lwc_string *imime_type, const http_parameter *params,
		llcache_handle *llcache, const char *fallback_charset,
		bool quirks, struct content **c)
{
	sprite_content *sprite;
	nserror error;

	sprite = talloc_zero(0, sprite_content);
	if (sprite == NULL)
		return NSERROR_NOMEM;

	error = content__init(&sprite->base, handler, imime_type, params,
			llcache, fallback_charset, quirks);
	if (error != NSERROR_OK) {
		talloc_free(sprite);
		return error;
	}

	*c = (struct content *) sprite;

	return NSERROR_OK;
}

/**
 * Convert a CONTENT_SPRITE for display.
 *
 * No conversion is necessary. We merely read the sprite dimensions.
 */

bool sprite_convert(struct content *c)
{
	sprite_content *sprite = (sprite_content *) c;
	os_error *error;
	int w, h;
	union content_msg_data msg_data;
	const char *source_data;
	unsigned long source_size;
	const void *sprite_data;
	char title[100];

	source_data = content__get_source_data(c, &source_size);

	sprite_data = source_data - 4;
	osspriteop_area *area = (osspriteop_area*) sprite_data;
	sprite->data = area;

	/* check for bad data */
	if ((int)source_size + 4 != area->used) {
		msg_data.error = messages_get("BadSprite");
		content_broadcast(c, CONTENT_MSG_ERROR, msg_data);
		return false;
	}

	error = xosspriteop_read_sprite_info(osspriteop_PTR,
			(osspriteop_area *)0x100,
			(osspriteop_id) ((char *) area + area->first),
			&w, &h, NULL, NULL);
	if (error) {
		LOG(("xosspriteop_read_sprite_info: 0x%x: %s",
				error->errnum, error->errmess));
		msg_data.error = error->errmess;
		content_broadcast(c, CONTENT_MSG_ERROR, msg_data);
		return false;
	}

	c->width = w;
	c->height = h;
	snprintf(title, sizeof(title), messages_get("SpriteTitle"), c->width,
				c->height, source_size);
	content__set_title(c, title);
	content_set_ready(c);
	content_set_done(c);
	/* Done: update status bar */
	content_set_status(c, "");
	return true;
}


/**
 * Destroy a CONTENT_SPRITE and free all resources it owns.
 */

void sprite_destroy(struct content *c)
{
	/* do not free c->data.sprite.data at it is simply a pointer to
	 * 4 bytes beforec->source_data. */
}


/**
 * Redraw a CONTENT_SPRITE.
 */

bool sprite_redraw(struct content *c, int x, int y,
		int width, int height, const struct rect *clip,
		float scale, colour background_colour)
{
	sprite_content *sprite = (sprite_content *) c;

	if (plot.flush && !plot.flush())
		return false;

	return image_redraw(sprite->data,
			ro_plot_origin_x + x * 2,
			ro_plot_origin_y - y * 2,
			width, height,
			c->width,
			c->height,
			background_colour,
			false, false, false,
			IMAGE_PLOT_OS);
}

nserror sprite_clone(const struct content *old, struct content **newc)
{
	sprite_content *sprite;
	nserror error;

	sprite = talloc_zero(0, sprite_content);
	if (sprite == NULL)
		return NSERROR_NOMEM;

	error = content__clone(old, &sprite->base);
	if (error != NSERROR_OK) {
		content_destroy(&sprite->base);
		return error;
	}

	/* Simply rerun convert */
	if (old->status == CONTENT_STATUS_READY ||
			old->status == CONTENT_STATUS_DONE) {
		if (sprite_convert(&sprite->base) == false) {
			content_destroy(&sprite->base);
			return NSERROR_CLONE_FAILED;
		}
	}

	*newc = (struct content *) sprite;

	return NSERROR_OK;
}

content_type sprite_content_type(lwc_string *mime_type)
{
	return CONTENT_IMAGE;
}

#endif


/**
 * Returns the bit depth of a sprite
 *
 * \param   s   sprite
 * \return  depth in bpp
 */

byte sprite_bpp(const osspriteop_header *s)
{
	/* bit 31 indicates the presence of a full alpha channel 
	 * rather than a binary mask */
	int type = ((unsigned)s->mode >> osspriteop_TYPE_SHIFT) & 15;
	byte bpp = 0;

	switch (type) {
		case osspriteop_TYPE_OLD:
		{
			bits psr;
			int val;
			if (!xos_read_mode_variable(s->mode, 
					os_MODEVAR_LOG2_BPP, &val, &psr) &&
					!(psr & _C))
				bpp = 1 << val;
		}
		break;
		case osspriteop_TYPE1BPP:  bpp = 1; break;
		case osspriteop_TYPE2BPP:  bpp = 2; break;
		case osspriteop_TYPE4BPP:  bpp = 4; break;
		case osspriteop_TYPE8BPP:  bpp = 8; break;
		case osspriteop_TYPE16BPP: bpp = 16; break;
		case osspriteop_TYPE32BPP: bpp = 32; break;
		case osspriteop_TYPE_CMYK: bpp = 32; break;
	}
	return bpp;
}
