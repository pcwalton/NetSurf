/*
 * This file is part of LibNSPNG.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <stdlib.h>
#include <string.h>

#include <libnspng.h>

#include "internal.h"
#include "liblzf/lzf.h"

static uint32_t get_interlaced_row(const nspng_image *image, uint32_t row,
		uint32_t pass)
{
	uint32_t result = (uint32_t) -1;

	if (row & 0x1) {
		if (pass == 6) {
			/* This pass runs over odd scanlines */
			result = image->passes[pass].idx + (row >> 1);
		}
	} else if ((row & 7) == 0) {
		if (pass == 0) {
			/* This pass runs over every 8th scanline */
			result = image->passes[pass].idx + (row >> 3);
		} else if (pass == 1 && image->width > 4) {
			/* This pass runs over every 8th scanline */
			result = image->passes[pass].idx + (row >> 3);
		} else if (pass == 3 && image->width > 2) {
			/* This pass runs over every 4th scanline */
			result = image->passes[pass].idx + (row >> 2);
		} else if (pass == 5 && image->width > 1) {
			/* This pass runs over every 2nd scanline */
			result = image->passes[pass].idx + (row >> 1);
		}
	} else if ((row & 3) == 0) {
		if (pass == 2) {
			/* This pass runs over every scanline that is a 
			 * multiple of 4 but not a multiple of 8. */
			result = image->passes[pass].idx + (row >> 2) - 
					(row >> 3) - 1;
		} else if (pass == 3 && image->width > 2) {
			/* This pass runs over every 4th scanline */
			result = image->passes[pass].idx + (row >> 2);
		} else if (pass == 5 && image->width > 1) {
			/* This pass runs over every 2nd scanline */
			result = image->passes[pass].idx + (row >> 1);
		}
	} else if ((row & 1) == 0) {
		if (pass == 4) {
			/* This pass runs over every scanline that is a
			 * multiple of 2 but not a multiple of 4. */
			result = image->passes[pass].idx + (row >> 1) - 
					(row >> 2) - 1;
		} else if (pass == 5 && image->width > 1) {
			/* This pass runs over every 2nd scanline */
			result = image->passes[pass].idx + (row >> 1);
		}
	}

	return result;
}

static void get_scanline_data(const nspng_image *image, uint32_t row,
		uint8_t *buf, const uint8_t **data, uint32_t *length)
{
	if (SCANLINE_IS_COMPRESSED(image, row)) {
		uint32_t offset = SCANLINE_OFFSET(image, row);
		uint32_t len = SCANLINE_LEN(image, row);

		len = lzf_decompress(image->data + offset, len,
				buf, image->bytes_per_scanline);

		*data = buf;
		*length = len;
	} else {
		*data = image->data + SCANLINE_OFFSET(image, row);
		*length = SCANLINE_LEN(image, row);
	}
}

static void process_scanline(nspng_ctx *ctx, const uint8_t *data, uint32_t len,
		uint8_t *scanline, uint32_t pass, uint32_t *rowbuf)
{
	static const uint32_t pix_init[7] = { 0, 4, 0, 2, 0, 1, 0 };
	static const uint32_t pix_step[7] = { 8, 8, 4, 4, 2, 2, 1 };
	const nspng_image *image = &ctx->image;
	const uint32_t colour_type = image->colour_type;
	const uint32_t bit_depth = image->bit_depth;
	const uint32_t width = image->width;
	const uint32_t bytes_per_pixel = image->bits_per_pixel >> 3;
	const uint32_t *palette = image->palette;
	const uint32_t step = pix_step[pass];
	uint32_t *curpix = rowbuf + pix_init[pass];
	int32_t aidx = 0 - image->filtered_byte_offset;
	uint32_t bytes_read_for_pixel = 0;
	uint32_t rgba = 0;

	for (uint32_t byte = 0; byte < len; byte++) {
		/* Reconstruct original byte value */
		uint32_t x = data[byte] + scanline[aidx++];

		scanline[byte] = x;

		/* Unpack pixel data from byte into rowbuf */
		if (colour_type != COLOUR_TYPE_INDEXED && bit_depth >= 8) {
			/* RGB / Greyscale (+ alpha) 8 & 16 bpc images */

			/* Taking only the even numbered bytes in the 16bpc 
			 * case results in some rudimentary downsampling. */
			if (bit_depth == 8 || (byte & 1) == 0) {
				rgba = (rgba << 8) | x;
			}

			if (++bytes_read_for_pixel == bytes_per_pixel) {
				/* Add default alpha if there isn't any */
				if ((colour_type & COLOUR_BITS_ALPHA) == 0) {
					rgba = (rgba << 8) | 0xff;
				}

				/* Promote greyscale images to RGB */
				if ((colour_type & COLOUR_BITS_TRUE) == 0) {
					uint32_t g;

					/* Currently 00GA, want GGGA */
					g = (rgba & 0xff00) << 8;
					g = g | (g << 8);
					rgba |= g;
				}

				*curpix = rgba;
				curpix += step;

				rgba = 0;
				bytes_read_for_pixel = 0;
			}
		} else {
			/* <8bpc greyscale or paletted images */
			for (uint32_t bit = 0; bit < 8 && 
				(uint32_t) (curpix - rowbuf) < width; 
					bit += bit_depth) {
				uint32_t index = (x >> (8 - bit - bit_depth)) & 
						((1 << bit_depth) - 1);

				*curpix = palette[index];
				curpix += step;
			}
		}
	}
}

/******************************************************************************
 * Public API                                                                 *
 ******************************************************************************/

nspng_error nspng_render(nspng_ctx *ctx, const nspng_rect *clip,
		nspng_row_callback callback, void *pw)
{
	const nspng_image *image = &ctx->image;
	uint8_t *scanline_buf;
	uint8_t *scanline;
	uint32_t *rowbuf;
	nspng_error error = NSPNG_OK;

	if (ctx == NULL || clip == NULL || callback == NULL)
		return NSPNG_BADPARM;

	if (ctx->state != STATE_HAD_IEND)
		return NSPNG_INVALID;

	/* Allocate filtered_byte_offset more bytes than needed, so there
	 * is no need to check for indices being within the array bounds */
	scanline_buf = ctx->alloc(NULL, 
			image->bytes_per_scanline + image->filtered_byte_offset,
			ctx->pw);
	if (scanline_buf == NULL)
		return NSPNG_NOMEM;
	/* Ensure these bytes are zero, as they are never written to */
	memset(scanline_buf, 0, image->filtered_byte_offset);

	rowbuf = ctx->alloc(NULL, image->width * sizeof(uint32_t), ctx->pw);
	if (rowbuf == NULL) {
		ctx->alloc(scanline_buf, 0, ctx->pw);
		return NSPNG_NOMEM;
	}

	scanline = scanline_buf + image->filtered_byte_offset;

	/* Process scanlines */
	for (uint32_t row = clip->y0; row < clip->y1; row++) {
		const uint8_t *data;
		uint32_t len;

		if (image->interlace) {
			/* Image is interlaced */
			for (uint32_t pass = 0; pass < 7; pass++) {
				uint32_t idx = 
					get_interlaced_row(image, row, pass);

				if (idx == (uint32_t) -1)
					continue;

				get_scanline_data(image, idx, scanline,
						&data, &len);

				process_scanline(ctx, data, len, scanline,
						pass, rowbuf);
			}
		} else {
			/* Image is not interlaced */
			get_scanline_data(image, row, scanline, 
					&data, &len);

			/* Just use the logic for pass 7 
			 * (i.e. all pixels in scanline) */
			process_scanline(ctx, data, len, scanline, 
					6, rowbuf);
		}

		error = callback((uint8_t *) rowbuf, 
				image->width * sizeof(uint32_t),
				row, 0, pw);
		if (error != NSPNG_OK) {
			break;
		}
	}

	ctx->alloc(rowbuf, 0, ctx->pw);
	ctx->alloc(scanline_buf, 0, ctx->pw);

	return error;
}

