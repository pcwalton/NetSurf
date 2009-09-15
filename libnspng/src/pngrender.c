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

static void process_scanline_slow(nspng_ctx *ctx, const uint8_t *data, 
		uint32_t len, uint32_t pass, uint8_t *rowbuf)
{
	/* LUTs for interlacing */
	/* Byte offsets for initial pixel in pass N */
	static const uint32_t pix_init[7] = { 0,  16,  0,  8, 0, 4, 0 };
	/* Number of bytes to skip between pixels in pass N */
	static const uint32_t pix_step[7] = { 28, 28, 12, 12, 4, 4, 0 };

	const nspng_image *image = &ctx->image;
	const uint32_t colour_type = image->colour_type;
	const uint32_t bit_depth = image->bit_depth;
	const uint32_t bytes_per_row = image->width * 4;
	const uint32_t bytes_per_pixel = image->bits_per_pixel >> 3;
	const uint32_t *palette = image->palette;
	const uint32_t step = pix_step[pass];
	uint8_t *curpix = rowbuf + pix_init[pass];
	uint32_t bytes_read_for_pixel = 0;
	uint32_t byte;

	for (byte = 0; byte < len; byte++) {
		/* Reconstruct original byte value */
		const uint8_t x = data[byte];

		/* Unpack pixel data from byte into rowbuf */
		if (colour_type != COLOUR_TYPE_INDEXED && bit_depth >= 8) {
			/* RGB / Greyscale (+ alpha) 8 & 16 bpc images */

			/* Taking only the even numbered bytes in the 16bpc 
			 * case results in some rudimentary downsampling. */
			if (bit_depth == 8 || (byte & 1) == 0) {
				*curpix++ = x;

				/* Promote greyscale images to RGB */
				if ((colour_type & COLOUR_BITS_TRUE) == 0 &&
						bytes_read_for_pixel == 0) {
					*curpix++ = x;
					*curpix++ = x;
				}
			}

			if (++bytes_read_for_pixel == bytes_per_pixel) {
				/* Add default alpha if there isn't any */
				if ((colour_type & COLOUR_BITS_ALPHA) == 0) {
					*curpix++ = 0xff;
				}

				curpix += step;

				bytes_read_for_pixel = 0;
			}
		} else {
			/* <8bpc greyscale or paletted images */
			for (uint32_t bit = 0; bit < 8 && 
				(uint32_t) (curpix - rowbuf) < bytes_per_row; 
					bit += bit_depth) {
				uint32_t index = (x >> (8 - bit - bit_depth)) & 
						((1 << bit_depth) - 1);
				uint32_t value = palette[index];

				/* Palette entries are RGBA */
				*curpix++ = (value >> 24) & 0xff;
				*curpix++ = (value >> 16) & 0xff;
				*curpix++ = (value >>  8) & 0xff;
				*curpix++ = (value >>  0) & 0xff;

				curpix += step;
			}
		}
	}
}

static void process_scanline(nspng_ctx *ctx, const uint8_t *data, uint32_t len,
		uint32_t pass, uint8_t *rowbuf)
{
	const uint32_t colour_type = ctx->image.colour_type;
	uint32_t byte;

	/* Optimise for non-interlaced, non-paletted, 8bpc scanlines */
	if (pass == 6 && ctx->image.bit_depth == 8 && 
			colour_type != COLOUR_TYPE_INDEXED) {
		if (colour_type == COLOUR_TYPE_RGBA) {
			memcpy(rowbuf, data, len);
		} else if (colour_type == COLOUR_TYPE_GREY_A) {
			for (byte = 0; byte < len - 1; byte += 2) {
				const uint8_t g = data[byte + 0];

				*rowbuf++ = g;
				*rowbuf++ = g;
				*rowbuf++ = g;
				*rowbuf++ = data[byte + 1];
			}
		} else if (colour_type == COLOUR_TYPE_RGB) {
			for (byte = 0; byte < len - 2; byte += 3) {
				*rowbuf++ = data[byte + 0];
				*rowbuf++ = data[byte + 1];
				*rowbuf++ = data[byte + 2];
				*rowbuf++ = 0xff;
			}
		} else if (colour_type == COLOUR_TYPE_GREY) {
			for (byte = 0; byte < len; byte++) {
				const uint8_t g = data[byte];

				*rowbuf++ = g;
				*rowbuf++ = g;
				*rowbuf++ = g;
				*rowbuf++ = 0xff;
			}
		}
	} else {
		/* Fall back to the slow code for the unhelpful cases */
		process_scanline_slow(ctx, data, len, pass, rowbuf);
	}
}

/******************************************************************************
 * Public API                                                                 *
 ******************************************************************************/

nspng_error nspng_render(nspng_ctx *ctx, const nspng_rect *clip,
		nspng_row_callback callback, void *pw)
{
	const nspng_image *image = &ctx->image;
	uint8_t *scanline;
	nspng_error error = NSPNG_OK;

	if (ctx == NULL || clip == NULL || callback == NULL)
		return NSPNG_BADPARM;

	if (ctx->state != STATE_HAD_IEND)
		return NSPNG_INVALID;

	scanline = ctx->src_scanline + image->filtered_byte_offset;

	if (ctx->rowbuf == NULL) {
		ctx->rowbuf = ctx->alloc(NULL, 
				image->width * sizeof(uint32_t), ctx->pw);
		if (ctx->rowbuf == NULL) {
			return NSPNG_NOMEM;
		}
	}

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

				process_scanline(ctx, data, len, pass, 
						ctx->rowbuf);
			}
		} else {
			/* Image is not interlaced */
			get_scanline_data(image, row, scanline, 
					&data, &len);

			/* Just use the logic for pass 7 
			 * (i.e. all pixels in scanline) */
			process_scanline(ctx, data, len, 6, ctx->rowbuf);
		}

		error = callback((uint8_t *) ctx->rowbuf, 
				image->width * sizeof(uint32_t),
				row, 0, pw);
		if (error != NSPNG_OK) {
			break;
		}
	}

	return error;
}

