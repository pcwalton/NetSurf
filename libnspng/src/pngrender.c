/*
 * This file is part of LibNSPNG.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <stdlib.h>

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
	} else if (row % 8 == 0) {
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
	} else if (row % 4 == 0) {
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
	} else if (row % 2 == 0) {
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
	uint32_t start = 0;
	uint32_t *curpix = rowbuf + pix_init[pass];

	for (uint32_t byte = 0; byte < len; byte++) {
		uint32_t a, x;

		a = (byte >= image->filtered_byte_offset) 
			? scanline[byte - image->filtered_byte_offset] 
			: 0;
		x = data[byte];

		/* Reconstruct original byte value */
		scanline[byte] = x + a;

		/* Unpack pixel data from byte into rowbuf */
		if (image->colour_type == COLOUR_TYPE_INDEXED ||
			(image->colour_type == COLOUR_TYPE_GREY && 
				image->bit_depth < 8)) {
			/* Paletted images */
			for (uint32_t bit = 0; bit < 8 && 
				(uint32_t) (curpix - rowbuf) < image->width; 
					bit += image->bit_depth) {
				uint32_t index = (scanline[byte] >> 
					(8 - bit - image->bit_depth)) & 
					((1 << image->bit_depth) - 1);

				*curpix = image->palette[index];
				curpix += pix_step[pass];
			}
		} else if ((image->colour_type & COLOUR_BITS_TRUE) == 0) {
			/* Greyscale (+ alpha) 8 & 16 bpc images */
			if (byte + 1 - start == (image->bits_per_pixel >> 3)) {
				uint32_t g, a;

				if (image->bit_depth == 8) {
					g = scanline[start];

					if (image->colour_type == 
							COLOUR_TYPE_GREY_A) {
						a = scanline[start + 1];
					} else {
						a = 0xff;
					}
				} else {
					/* Downsample to 8bpc */
					g = scanline[start];

					if (image->colour_type ==
							COLOUR_TYPE_GREY_A) {
						a = scanline[start + 2];
					} else {
						a = 0xff;
					}
				}

				*curpix = (g << 24) | (g << 16) | (g << 8) | a;
				curpix += pix_step[pass];

				start = byte + 1;
			}
		} else {
			/* 8/16 bpc RGB(A) images */
			if (byte + 1 - start == (image->bits_per_pixel >> 3)) {
				uint32_t r, g, b, a;

				if (image->bit_depth == 8) {
					r = scanline[start];
					g = scanline[start + 1];
					b = scanline[start + 2];

					if (image->colour_type == 
							COLOUR_TYPE_GREY_A) {
						a = scanline[start + 3];
					} else {
						a = 0xff;
					}
				} else {
					/* Downsample to 8bpc */
					r = scanline[start];
					g = scanline[start + 2];
					b = scanline[start + 4];

					if (image->colour_type ==
							COLOUR_TYPE_GREY_A) {
						a = scanline[start + 6];
					} else {
						a = 0xff;
					}
				}

				*curpix = (r << 24) | (g << 16) | (b << 8) | a;
				curpix += pix_step[pass];

				start = byte + 1;
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
	uint32_t *rowbuf;
	nspng_error error = NSPNG_OK;

	if (ctx == NULL || clip == NULL || callback == NULL)
		return NSPNG_BADPARM;

	if (ctx->state != STATE_HAD_IEND)
		return NSPNG_INVALID;

	scanline_buf = ctx->alloc(NULL, image->bytes_per_scanline, ctx->pw);
	if (scanline_buf == NULL)
		return NSPNG_NOMEM;

	rowbuf = ctx->alloc(NULL, image->width * sizeof(uint32_t), ctx->pw);
	if (rowbuf == NULL) {
		ctx->alloc(scanline_buf, 0, ctx->pw);
		return NSPNG_NOMEM;
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

				get_scanline_data(image, idx, scanline_buf,
						&data, &len);

				process_scanline(ctx, data, len, scanline_buf,
						pass, rowbuf);
			}
		} else {
			/* Image is not interlaced */
			get_scanline_data(image, row, scanline_buf, 
					&data, &len);

			/* Just use the logic for pass 7 
			 * (i.e. all pixels in scanline) */
			process_scanline(ctx, data, len, scanline_buf, 
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

