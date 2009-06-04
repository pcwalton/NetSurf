/*
 * This file is part of LibNSPNG.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <libnspng.h>

#include "internal.h"

#include "liblzf/lzf.h"

typedef enum nspng_chunk_type {
	CHUNK_IHDR = ('I' << 24) | ('H' << 16) | ('D' << 8) | 'R',
	CHUNK_PLTE = ('P' << 24) | ('L' << 16) | ('T' << 8) | 'E',
	CHUNK_IDAT = ('I' << 24) | ('D' << 16) | ('A' << 8) | 'T',
	CHUNK_IEND = ('I' << 24) | ('E' << 16) | ('N' << 8) | 'D',

	CHUNK_cHRM = ('c' << 24) | ('H' << 16) | ('R' << 8) | 'M',
	CHUNK_gAMA = ('g' << 24) | ('A' << 16) | ('M' << 8) | 'A',
	CHUNK_iCCP = ('i' << 24) | ('C' << 16) | ('C' << 8) | 'P',
	CHUNK_sBIT = ('s' << 24) | ('B' << 16) | ('I' << 8) | 'T',
	CHUNK_sRGB = ('s' << 24) | ('R' << 16) | ('G' << 8) | 'B',

	CHUNK_bKGD = ('b' << 24) | ('K' << 16) | ('G' << 8) | 'D',
	CHUNK_hIST = ('h' << 24) | ('I' << 16) | ('S' << 8) | 'T',
	CHUNK_tRNS = ('t' << 24) | ('R' << 16) | ('N' << 8) | 'S',

	CHUNK_pHYs = ('p' << 24) | ('H' << 16) | ('Y' << 8) | 's',
	CHUNK_sPLT = ('s' << 24) | ('P' << 16) | ('L' << 8) | 'T',

	CHUNK_tIME = ('t' << 24) | ('I' << 16) | ('M' << 8) | 'E',
	CHUNK_iTXt = ('i' << 24) | ('T' << 16) | ('X' << 8) | 't',
	CHUNK_tEXt = ('t' << 24) | ('E' << 16) | ('X' << 8) | 't',
	CHUNK_zTXt = ('z' << 24) | ('T' << 16) | ('X' << 8) | 't'
} nspng_chunk_type;

/**
 * PNG 8 byte signature
 */
static const uint8_t png_sig[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };

/**
 * 1bpp Greyscale palette
 */
static uint32_t g1bpp[2] = { 0x000000ff, 0xffffffff };

/**
 * 2bpp Greyscale palette
 */
static uint32_t g2bpp[4] = {
	0x000000ff, 0x555555ff, 0xaaaaaaff, 0xffffffff 
};

/**
 * 4bpp Greyscale palette
 */
static uint32_t g4bpp[16] = { 
	0x000000ff, 0x111111ff, 0x222222ff, 0x333333ff,
	0x444444ff, 0x555555ff, 0x666666ff, 0x777777ff,
	0x888888ff, 0x999999ff, 0xaaaaaaff, 0xbbbbbbff,
	0xccccccff, 0xddddddff, 0xeeeeeeff, 0xffffffff
};

/**
 * Read a word from the datastream. Assumes there's at least 4 bytes available
 *
 * \param data  Pointer to current position in datastream
 * \return 32bit value
 */
static inline uint32_t read_word(const uint8_t *data)
{
	/* All integers in a PNG datastream are big-endian */
	return (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
}

/**
 * Read a halfword from the datastream. 
 * Assumes there's at least 2 bytes available
 *
 * \param data  Pointer to current position in datastream
 * \return 16bit value
 */
static inline uint16_t read_halfword(const uint8_t *data)
{
	/* All integers in a PNG datastream are big-endian */
	return (data[0] << 8) | data[1];
}

/**
 * Read a chunk from the data stream
 *
 * \param ctx  Decoder context
 * \param data  Start of chunk to read
 * \param len   Length, in bytes, of available data
 * \return NSPNG_OK on success.
 */
static nspng_error read_chunk(nspng_ctx *ctx, const uint8_t *data, size_t len)
{
	assert(ctx != NULL);
	assert(data != NULL);

	while (len > 0 && ctx->chunk_state != CHUNK_STATE_HAD_CRC) {
		ctx->chunk.amount_read++;

		if (ctx->chunk_state == CHUNK_STATE_START) {
			ctx->chunk.length = (ctx->chunk.length << 8) | *data;

			if (ctx->chunk.amount_read == 4) {
				/* Length should not exceed 2^31 - 1 bytes */
				if (ctx->chunk.length > 0x7fffffff) {
					return NSPNG_INVALID;
				}

				/* Initialise CRC */
				ctx->chunk.computed_crc = crc32(0L, Z_NULL, 0);

				ctx->chunk_state = CHUNK_STATE_HAD_LENGTH;
			}
		} else if (ctx->chunk_state == CHUNK_STATE_HAD_LENGTH) {
			ctx->chunk.type = (ctx->chunk.type << 8) | *data;

			/* CRC includes chunk type */
			ctx->chunk.computed_crc = crc32(
					ctx->chunk.computed_crc, data, 1);

			if (ctx->chunk.amount_read == 8) {
				if (ctx->chunk.length > ctx->chunk.data_alloc) {
					uint8_t *temp = ctx->alloc(
							ctx->chunk.data, 
							ctx->chunk.length, 
							ctx->pw);
					if (temp == NULL)
						return NSPNG_NOMEM;

					ctx->chunk.data = temp;
					ctx->chunk.data_alloc = 
							ctx->chunk.length;
				}

				if (ctx->chunk.length == 0) {
					ctx->chunk_state = CHUNK_STATE_HAD_DATA;
				} else {
					ctx->chunk_state = CHUNK_STATE_HAD_TYPE;
				}
			}
		} else if (ctx->chunk_state == CHUNK_STATE_HAD_TYPE) {
			ctx->chunk.data[ctx->chunk.amount_read - 9] = *data;

			ctx->chunk.computed_crc = crc32(
					ctx->chunk.computed_crc, data, 1);

			if (ctx->chunk.amount_read == ctx->chunk.length + 8) {
				ctx->chunk_state = CHUNK_STATE_HAD_DATA;
			}
		} else if (ctx->chunk_state == CHUNK_STATE_HAD_DATA) {
			ctx->chunk.crc = (ctx->chunk.crc << 8) | *data;

			if (ctx->chunk.amount_read == ctx->chunk.length + 12) {
				/* Ensure CRCs match */
				if (ctx->chunk.computed_crc != ctx->chunk.crc) {
					return NSPNG_INVALID;
				}

				ctx->chunk_state = CHUNK_STATE_HAD_CRC;
			}
		}

		data++;
		len--;
	}

	return (ctx->chunk_state == CHUNK_STATE_HAD_CRC) ? NSPNG_OK 
							 : NSPNG_NEEDDATA;
}

static nspng_error process_ihdr(nspng_ctx *ctx)
{
	const nspng_chunk *chunk;
	uint32_t width, height;
	uint32_t bit_depth, colour_type, compression, filter, interlace;
	uint32_t n_scanlines = 0;
	uint32_t bpp;

	assert(ctx != NULL);
	assert(ctx->chunk.type == CHUNK_IHDR);

	chunk = &ctx->chunk;

	/* Ignore any spurious IHDR chunks */
	if (ctx->state >= STATE_HAD_IHDR)
		return NSPNG_OK;

	/* IHDR chunk must be 13 bytes long */
	if (chunk->length != 13) {
		return NSPNG_INVALID;
	}

	/* Extract fields */
	width = read_word(chunk->data);
	height = read_word(chunk->data + 4);
	bit_depth = chunk->data[8];
	colour_type = chunk->data[9];
	compression = chunk->data[10];
	filter = chunk->data[11];
	interlace = chunk->data[12];

	/* Validate data */
	if (colour_type != COLOUR_TYPE_GREY && 
			colour_type != COLOUR_TYPE_RGB &&
			colour_type != COLOUR_TYPE_INDEXED &&
			colour_type != COLOUR_TYPE_GREY_A &&
			colour_type != COLOUR_TYPE_RGBA) {
		return NSPNG_INVALID;
	}

	if (bit_depth == 1 || bit_depth == 2 || bit_depth == 4) {
		if (colour_type != COLOUR_TYPE_GREY && 
				colour_type != COLOUR_TYPE_INDEXED) {
			return NSPNG_INVALID;
		}
	} else if (bit_depth == 8) {
		/* Valid for all */
	} else if (bit_depth == 16) {
		if (colour_type == COLOUR_TYPE_INDEXED) {
			return NSPNG_INVALID;
		}
	} else {
		return NSPNG_INVALID;
	}

	if (compression != COMPRESSION_TYPE_DEFLATE) {
		return NSPNG_INVALID;
	}

	if (filter != FILTER_METHOD_ADAPTIVE) {
		return NSPNG_INVALID;
	}

	if (interlace != INTERLACE_TYPE_NONE && 
			interlace != INTERLACE_TYPE_ADAM7) {
		return NSPNG_INVALID;
	}

	ctx->image.width = width;
	ctx->image.height = height;
	ctx->image.bit_depth = bit_depth;
	ctx->image.colour_type = colour_type;
	ctx->image.compression = compression;
	ctx->image.filter = filter;
	ctx->image.interlace = interlace;

	/* Create our own palette for <8bpp greyscale images */
	if (colour_type == COLOUR_TYPE_GREY && bit_depth < 8) {
		if (bit_depth == 1) {
			ctx->image.palette = g1bpp;
			ctx->image.palette_entries = 2;
		} else if (bit_depth == 2) {
			ctx->image.palette = g2bpp;
			ctx->image.palette_entries = 4;
		} else {
			ctx->image.palette = g4bpp;
			ctx->image.palette_entries = 16;
		}
	}

#define ROUND(x, n) (((x) + ((n) - 1)) & ~((n) - 1))

	/* Calculate byte length of a scanline */
	if (colour_type == COLOUR_TYPE_GREY || 
			colour_type == COLOUR_TYPE_INDEXED) {
		bpp = bit_depth;
	} else if (colour_type == COLOUR_TYPE_GREY_A) {
		bpp = 2 * bit_depth;
	} else if (colour_type == COLOUR_TYPE_RGB) {
		bpp = 3 * bit_depth;
	} else /* if (colour_type == COLOUR_TYPE_RGBA) */ {
		bpp = 4 * bit_depth;
	}

	ctx->image.bits_per_pixel = bpp;
	ctx->image.bytes_per_scanline = ROUND(width * bpp, 8) >> 3;

	/* Calculate filtered byte offset */
	if (colour_type == COLOUR_TYPE_GREY && bit_depth >= 8) {
		ctx->image.filtered_byte_offset = 1 * (bit_depth >> 3);
	} else if (colour_type == COLOUR_TYPE_GREY_A) {
		ctx->image.filtered_byte_offset = 2 * (bit_depth >> 3);
	} else if (colour_type == COLOUR_TYPE_RGB) {
		ctx->image.filtered_byte_offset = 3 * (bit_depth >> 3);
	} else if (colour_type == COLOUR_TYPE_RGBA) {
		ctx->image.filtered_byte_offset = 4 * (bit_depth >> 3);
	} else {
		/* Indexed or 1/2/4bpp greyscale */
		ctx->image.filtered_byte_offset = 1;
	}

	/* Compute total number of scanlines in image */
	if (interlace) {
		/* Image is interlaced, thus constructed from 7 sub-images.
		 * Calculate the number of scanlines, and bytes per scanline
		 * for each sub-image, and use the sum of all sub-image
		 * scanlines as the number of scanlines for the complete
		 * image.
		 */
		if (width > 0 && height > 0) {
			/* Pass 1 */
			ctx->image.passes[0].idx = n_scanlines;
			ctx->image.passes[0].bps = 
				ROUND(bpp * (ROUND(width, 8) >> 3), 8) >> 3;
			n_scanlines += ROUND(height, 8) >> 3;
		}

		if (width > 4) {
			/* Pass 2 */
			ctx->image.passes[1].idx = n_scanlines;
			ctx->image.passes[1].bps =
				ROUND(bpp * (ROUND(width - 4, 8) >> 3), 8) >> 3;
			n_scanlines += ROUND(height, 8) >> 3;
		}

		if (height > 4) {
			/* Pass 3 */
			ctx->image.passes[2].idx = n_scanlines;
			ctx->image.passes[2].bps =
				ROUND(bpp * (ROUND(width, 4) >> 2), 8) >> 3;
			n_scanlines += ROUND(height - 4, 8) >> 3;
		}

		if (width > 2) {
			/* Pass 4 */
			ctx->image.passes[3].idx = n_scanlines;
			ctx->image.passes[3].bps =
				ROUND(bpp * (ROUND(width - 2, 4) >> 2), 8) >> 3;
			n_scanlines += ROUND(height, 4) >> 2;
		}

		if (height > 2) {
			/* Pass 5 */
			ctx->image.passes[4].idx = n_scanlines;
			ctx->image.passes[4].bps =
				ROUND(bpp * (ROUND(width, 2) >> 1), 8) >> 3;
			n_scanlines += ROUND(height - 2, 4) >> 2;
		}

		if (width > 1) {
			/* Pass 6 */
			ctx->image.passes[5].idx = n_scanlines;
			ctx->image.passes[5].bps =
					ROUND(bpp * (width >> 1), 8) >> 3;
			n_scanlines += ROUND(height, 2) >> 1;
		}

		if (height > 1) {
			/* Pass 7 */
			ctx->image.passes[6].idx = n_scanlines;
			ctx->image.passes[6].bps = ROUND(bpp * width, 8) >> 3;
			n_scanlines += height >> 1;
		}
	} else {
		/* Non interlaced -- simply use image height */
		n_scanlines = height;
	}

	ctx->image.n_scanlines = n_scanlines;

	/* Create scanline index */
	ctx->image.scanline_idx = ctx->alloc(NULL, 
			n_scanlines * sizeof(uint32_t), ctx->pw);
	if (ctx->image.scanline_idx == NULL) {
		return NSPNG_NOMEM;
	}

	memset(ctx->image.scanline_idx, 0, height * sizeof(uint32_t));

#undef ROUND

	return NSPNG_OK;
}

static nspng_error process_idat(nspng_ctx *ctx)
{
	const nspng_chunk *chunk;
	nspng_image *image;
	uint8_t buf[8192];
	uint8_t *temp;
	int zlib_ret;

	assert(ctx != NULL);
	assert(ctx->chunk.type == CHUNK_IDAT);

	chunk = &ctx->chunk;
	image = &ctx->image;

	/* Create scanline buffers */
	if (ctx->src_scanline == NULL) {
		ctx->src_scanline = ctx->alloc(NULL, image->bytes_per_scanline, 
				ctx->pw);
		if (ctx->src_scanline == NULL) {
			return NSPNG_NOMEM;
		}
	}

	if (ctx->dst_scanline == NULL) {
		ctx->dst_scanline = ctx->alloc(NULL, image->bytes_per_scanline, 
				ctx->pw);
		if (ctx->dst_scanline == NULL) {
			return NSPNG_NOMEM;
		}
	}

	/* Set up the input */
	ctx->zlib_stream.avail_in = chunk->length;
	ctx->zlib_stream.next_in = chunk->data;

	/* Decompress into the output buffer */
	do {
		ctx->zlib_stream.avail_out = sizeof(buf);
		ctx->zlib_stream.next_out = buf;

		/* Decompress until output buffer is full */
		do {
			zlib_ret = inflate(&ctx->zlib_stream, Z_SYNC_FLUSH);
			assert(zlib_ret != Z_STREAM_ERROR);
			/* Convert Z_NEED_DICT into a data error as we 
			 * know nothing of the required dictionary. */
			if (zlib_ret == Z_NEED_DICT) {
				zlib_ret = Z_DATA_ERROR;
			} 
		} while (zlib_ret == Z_OK && ctx->zlib_stream.avail_out > 0);

		/* Process scanlines */
		for (uint32_t idx = 0; 
				idx < sizeof(buf) - ctx->zlib_stream.avail_out; 
				idx++) {
			uint32_t bps = 0;
			bool first = false;

			/* Retrieve number of bytes to process in current 
			 * scanline. Also determine if the current scanline
			 * is the first in the (sub) image */
			if (image->interlace) {
				/* Image is interlaced */
				if (image->height > 1 && ctx->cur_scanline >= 
						image->passes[6].idx) {
					/* Pass 7 */
					bps = image->passes[6].bps;
					first = (ctx->cur_scanline == 
							image->passes[6].idx);
				} else if (image->width > 1 &&
						ctx->cur_scanline >= 
						image->passes[5].idx) {
					/* Pass 6 */
					bps = image->passes[5].bps;
					first = (ctx->cur_scanline == 
							image->passes[5].idx);
				} else if (image->height > 2 &&
						ctx->cur_scanline >= 
						image->passes[4].idx) {
					/* Pass 5 */
					bps = image->passes[4].bps;
					first = (ctx->cur_scanline == 
							image->passes[4].idx);
				} else if (image->width > 2 &&
						ctx->cur_scanline >= 
						image->passes[3].idx) {
					/* Pass 4 */
					bps = image->passes[3].bps;
					first = (ctx->cur_scanline == 
							image->passes[3].idx);
				} else if (image->height > 4 &&
						ctx->cur_scanline >= 
						image->passes[2].idx) {
					/* Pass 3 */
					bps = image->passes[2].bps;
					first = (ctx->cur_scanline == 
							image->passes[2].idx);
				} else if (image->width > 4 &&
						ctx->cur_scanline >= 
						image->passes[1].idx) {
					/* Pass 2 */
					bps = image->passes[1].bps;
					first = (ctx->cur_scanline == 
							image->passes[1].idx);
				} else if (image->width > 0 && 
						image->height > 0 && 
						ctx->cur_scanline >= 
						image->passes[0].idx) {
					/* Pass 1 */
					bps = image->passes[0].bps;
					first = (ctx->cur_scanline == 
							image->passes[0].idx);
				}
			} else {
				/* Non-interlaced image */
				bps = image->bytes_per_scanline;
				first = (ctx->cur_scanline == 0);
			}

			if (ctx->bytes_read_for_scanline == 0) {
				/* Filter type byte */
				ctx->scanline_filter = buf[idx];
			} else {
				/* Scanline data */
				uint32_t i = ctx->bytes_read_for_scanline - 1;
				uint32_t fbo = image->filtered_byte_offset;
				uint32_t a, b, c, x;

				a = (i >= fbo) ? ctx->src_scanline[i - fbo] : 0;
				b = (first == false) ? ctx->src_scanline[i] : 0;
				c = (first == false && i >= fbo) 
					? ctx->prev_pixel[i % fbo] : 0;
				x = buf[idx];

				ctx->prev_pixel[i % fbo] = ctx->src_scanline[i];

				/* Reconstruct original byte value */
				if (ctx->scanline_filter == 
						ADAPTIVE_FILTER_NONE) {
					ctx->src_scanline[i] = x;
				} else if (ctx->scanline_filter == 
						ADAPTIVE_FILTER_SUB) {
					ctx->src_scanline[i] = x + a;
				} else if (ctx->scanline_filter == 
						ADAPTIVE_FILTER_UP) {
					ctx->src_scanline[i] = x + b;
				} else if (ctx->scanline_filter == 
						ADAPTIVE_FILTER_AVERAGE) {
					ctx->src_scanline[i] = 
							x + ((a + b) >> 1);
				} else /* if (ctx->scanline_filter == 
						ADAPTIVE_FILTER_PAETH) */ {
					uint32_t p = a + b - c;
					uint32_t pa = abs(p - a);
					uint32_t pb = abs(p - b);
					uint32_t pc = abs(p - c);

					if (pa <= pb && pa <= pc) {
						ctx->src_scanline[i] = x + a;
					} else if (pb <= pc) {
						ctx->src_scanline[i] = x + b;
					} else {
						ctx->src_scanline[i] = x + c;
					}
				}

				/* Re-encode using SUB filter */
				if (i < fbo) {
					ctx->dst_scanline[i] = 
						ctx->src_scanline[i];
				} else {
					ctx->dst_scanline[i] = 
						ctx->src_scanline[i] -
						ctx->src_scanline[i - fbo];
				}
			}

			if (++ctx->bytes_read_for_scanline > bps) { 
				uint32_t written;

				/* Create/extend image buffer */
#define OUTPUT_CHUNK_SIZE 8192
				while (image->data_len + bps > 
						image->data_alloc) {
					temp = ctx->alloc(image->data, 
							image->data_alloc + 
							OUTPUT_CHUNK_SIZE, 
							ctx->pw);
					if (temp == NULL) {
						return NSPNG_NOMEM;
					}

					image->data = temp;
					image->data_alloc += OUTPUT_CHUNK_SIZE;
				}
#undef OUTPUT_CHUNK_SIZE

				/* Compress scanline into buffer */
				written = lzf_compress(ctx->dst_scanline, 
						bps, 
						image->data + image->data_len, 
						bps - 1);
				if (written == 0) {
					/* Would be larger - use uncompressed */
					memcpy(image->data + image->data_len,
						ctx->dst_scanline,
						bps);
				}

				/* Write index, flagging compressed */
				image->scanline_idx[ctx->cur_scanline++] =
					image->data_len | 
					((written != 0) 
						? SCANLINE_COMPRESSED_FLAG : 0);

				/* Add scanline data to total length */
				image->data_len += (written == 0) 
						? bps 
						: written;

				/* Maximum permissible bytes in decoded 
				 * image is 2^31-1 as we use bit 31 to store
				 * the compression flag */
				if (image->data_len > INT32_MAX)
					return NSPNG_INVALID;

				/* Reset for next scanline */
				ctx->bytes_read_for_scanline = 0;
			}
		}
	} while (zlib_ret == Z_OK);

	if (zlib_ret == Z_DATA_ERROR) {
		return NSPNG_INVALID;
	} else if (zlib_ret == Z_MEM_ERROR) {
		return NSPNG_NOMEM;
	}

	if (image->data_len > 0) {
		/* Shrink image buffer down to its actual size */
		temp = ctx->alloc(image->data, image->data_len, ctx->pw);
		if (temp == NULL) {
			return NSPNG_NOMEM;
		}
		image->data = temp;
		image->data_alloc = image->data_len;
	}

	return NSPNG_OK;
}

static nspng_error process_plte(nspng_ctx *ctx)
{
	const nspng_chunk *chunk;
	nspng_image *image;
	uint32_t n_entries;
	uint32_t *palette;
	uint32_t i;

	assert(ctx != NULL);
	assert(ctx->chunk.type == CHUNK_PLTE);

	chunk = &ctx->chunk;
	image = &ctx->image;

	/* Ignore spurious PLTE chunks */
	if (ctx->state >= STATE_HAD_PLTE) {
		return NSPNG_OK;
	}

	/* Chunk length must be a multiple of 3 */
	if ((chunk->length % 3) != 0) {
		return NSPNG_INVALID;
	}

	/* PLTE is not permitted for greyscale images */
	if ((image->colour_type & COLOUR_BITS_TRUE) == 0) {
		return NSPNG_INVALID;
	}

	n_entries = chunk->length / 3;

	if (image->colour_type == COLOUR_TYPE_INDEXED) {
		/* Number of palette entries must fit in bit depth */
		if (n_entries > (1u << image->bit_depth)) {
			return NSPNG_INVALID;
		}
	} else if (n_entries > 256) {
		/* 256 entries maximum */
		return NSPNG_INVALID;
	}

	/* Create and populate palette */
	palette = ctx->alloc(NULL, n_entries * sizeof(uint32_t), ctx->pw);
	if (palette == NULL) {
		return NSPNG_NOMEM;
	}

	for (i = 0; i < n_entries; i++) {
		/* RrGgBbAa */
		palette[i] = (chunk->data[i * 3 + 0] << 24) |
			     (chunk->data[i * 3 + 1] << 16) |
			     (chunk->data[i * 3 + 2] <<  8) |
			     0xff /* Fully opaque by default */;
	}

	image->palette_entries = n_entries;
	image->palette = palette;

	return NSPNG_OK;
}

static nspng_error process_gama(nspng_ctx *ctx)
{
	const nspng_chunk *chunk;
	nspng_image *image;

	assert(ctx != NULL);
	assert(ctx->chunk.type == CHUNK_gAMA);

	chunk = &ctx->chunk;
	image = &ctx->image;

	/* Ignore gAMA chunks that occur after PLTE/IDAT */
	if (ctx->state >= STATE_HAD_PLTE) {
		return NSPNG_OK;
	}

	/* Image gamma already set, this must be a spurious gAMA chunk */
	if (image->had_gama != 0) {
		return NSPNG_OK;
	}

	/* gAMA chunk must be 4 bytes long */
	if (chunk->length != 4) {
		return NSPNG_INVALID;
	}

	image->gamma = read_word(chunk->data);

	image->had_gama = 1;

	return NSPNG_OK;
}

static nspng_error process_bkgd(nspng_ctx *ctx)
{
	const nspng_chunk *chunk;
	nspng_image *image;

	assert(ctx != NULL);
	assert(ctx->chunk.type == CHUNK_bKGD);

	chunk = &ctx->chunk;
	image = &ctx->image;

	/* Ignore bKGD chunks that occur after IDAT */
	if (ctx->state >= STATE_HAD_IDAT) {
		return NSPNG_OK;
	}

	/* Image background already set, this must be a spurious bKGD chunk */
	if (image->had_bkgd != 0) {
		return NSPNG_OK;
	}

	if ((image->colour_type & COLOUR_BITS_TRUE) == 0) { 
		if (chunk->length != 2) {
			return NSPNG_INVALID;
		}

		image->background.r = read_halfword(chunk->data);
		image->background.g = image->background.r;
		image->background.b = image->background.r;
	} else if (image->colour_type == COLOUR_TYPE_INDEXED) {
		uint32_t rgba;

		if (chunk->length != 1 || 
				chunk->data[0] > image->palette_entries) {
			return NSPNG_INVALID;
		}

		rgba = image->palette[chunk->data[0]];

		image->background.r = (rgba >> 24) & 0xff;
		image->background.g = (rgba >> 16) & 0xff;
		image->background.b = (rgba >>  8) & 0xff;
	} else /* if ((image->colour_type & COLOUR_BITS_TRUE) != 0) */ {
		if (chunk->length != 6) {
			return NSPNG_INVALID;
		}

		image->background.r = read_halfword(chunk->data);
		image->background.g = read_halfword(chunk->data + 2);
		image->background.b = read_halfword(chunk->data + 4);
	}

	image->had_bkgd = 1;

	return NSPNG_OK;
}


static nspng_error process_trns(nspng_ctx *ctx)
{
	const nspng_chunk *chunk;
	nspng_image *image;

	assert(ctx != NULL);
	assert(ctx->chunk.type == CHUNK_tRNS);

	chunk = &ctx->chunk;
	image = &ctx->image;

	/* Ignore tRNS chunks that occur after IDAT */
	if (ctx->state >= STATE_HAD_IDAT) {
		return NSPNG_OK;
	}

	/* Image transparency already set, this must be a spurious tRNS chunk */
	if (image->had_trns != 0) {
		return NSPNG_OK;
	}

	/* tRNS is not permitted with full-alpha images */
	if (image->colour_type == COLOUR_TYPE_RGBA || 
			image->colour_type == COLOUR_TYPE_GREY_A) {
		return NSPNG_INVALID;
	} else if (image->colour_type == COLOUR_TYPE_GREY) {
		if (chunk->length != 2) {
			return NSPNG_INVALID;
		}

		image->transparency.r = read_halfword(chunk->data);
		image->transparency.g = image->transparency.r;
		image->transparency.b = image->transparency.r;
	} else if (image->colour_type == COLOUR_TYPE_RGB) {
		if (chunk->length != 6) {
			return NSPNG_INVALID;
		}

		image->transparency.r = read_halfword(chunk->data);
		image->transparency.g = read_halfword(chunk->data + 2);
		image->transparency.b = read_halfword(chunk->data + 4);
	} else /* if (image->colour_type == COLOUR_TYPE_INDEXED) */ {
		uint32_t i;

		if (chunk->length > image->palette_entries) {
			return NSPNG_INVALID;
		}

		for (i = 0; i < chunk->length; i++) {
			image->palette[i] = (image->palette[i] & ~0xff) |
					    chunk->data[i];
		}		
	}

	image->had_trns = 1;

	return NSPNG_OK;
}

static nspng_error process_chunk(nspng_ctx *ctx)
{
	nspng_error error = NSPNG_OK;

	if (ctx->chunk.type == CHUNK_IHDR) {
		error = process_ihdr(ctx);
	} else if (ctx->chunk.type == CHUNK_IDAT) {
		error = process_idat(ctx);
	} else if (ctx->chunk.type == CHUNK_PLTE) {
		error = process_plte(ctx);
	} else if (ctx->chunk.type == CHUNK_gAMA) {
		error = process_gama(ctx);
	} else if (ctx->chunk.type == CHUNK_bKGD) {
		error = process_bkgd(ctx);
	} else if (ctx->chunk.type == CHUNK_tRNS) {
		error = process_trns(ctx);
	}

	/* Calculate state transitions */
	if (ctx->state == STATE_VERIFIED_SIGNATURE) {
		/* Must be IHDR */
		if (ctx->chunk.type == CHUNK_IHDR) {
			ctx->state = STATE_HAD_IHDR;
		} else {
			error = NSPNG_INVALID;
		}
	} else if (ctx->state == STATE_HAD_IHDR) {
		if (ctx->chunk.type == CHUNK_PLTE) {
			ctx->state = STATE_HAD_PLTE;
		} else if (ctx->chunk.type == CHUNK_IDAT) {
			if (ctx->image.colour_type == COLOUR_TYPE_INDEXED) {
				/* Indexed images must have PLTE */
				error = NSPNG_INVALID;
			} else {
				ctx->state = STATE_HAD_IDAT;
			}
		} else if (ctx->chunk.type == CHUNK_IEND) {
			ctx->state = STATE_HAD_IEND;
		}
	} else if (ctx->state == STATE_HAD_PLTE) {
		if (ctx->chunk.type == CHUNK_IDAT) {
			ctx->state = STATE_HAD_IDAT;
		} else if (ctx->chunk.type == CHUNK_IEND) {
			ctx->state = STATE_HAD_IEND;
		}
	} else /*if (ctx->state == STATE_HAD_IDAT)*/ {
		if (ctx->chunk.type == CHUNK_IEND) {
			ctx->state = STATE_HAD_IEND;
		}
	}

	/* Reset chunk for next time */
	ctx->chunk.length = 0;
	ctx->chunk.type = 0;
	ctx->chunk.crc = 0;
	ctx->chunk.amount_read = 0;
	ctx->chunk_state = CHUNK_STATE_START;

	return error;
}

static void *nspng_zalloc(void *ctx, uInt items, uInt size)
{
	nspng_ctx *c = ctx;
	void *result;

	result = c->alloc(NULL, items * size, c->pw);

	return (result == NULL) ? Z_NULL : result;
}

static void nspng_zfree(void *ctx, void *ptr)
{
	nspng_ctx *c = ctx;

	if (ptr != NULL)
		c->alloc(ptr, 0, c->pw);
}

/******************************************************************************
 * Public API                                                                 *
 ******************************************************************************/

nspng_error nspng_ctx_create(nspng_allocator_fn alloc, void *pw, 
		nspng_ctx **result)
{
	nspng_ctx *ctx;
	int zlib_ret;

	if (alloc == NULL || result == NULL)
		return NSPNG_BADPARM;

	ctx = alloc(NULL, sizeof(nspng_ctx), pw);
	if (ctx == NULL)
		return NSPNG_NOMEM;

	memset(ctx, 0, sizeof(nspng_ctx));

	ctx->state = STATE_START;

	ctx->zlib_stream.next_in = Z_NULL;
	ctx->zlib_stream.avail_in = 0;
	ctx->zlib_stream.zalloc = nspng_zalloc;
	ctx->zlib_stream.zfree = nspng_zfree;
	ctx->zlib_stream.opaque = ctx;

	ctx->alloc = alloc;
	ctx->pw = pw;

	zlib_ret = inflateInit(&ctx->zlib_stream);
	if (zlib_ret != Z_OK) {
		alloc(ctx, 0, pw);
		return NSPNG_NOMEM;
	}

	*result = ctx;

	return NSPNG_OK;
}

nspng_error nspng_ctx_destroy(nspng_ctx *ctx)
{
	if (ctx == NULL)
		return NSPNG_BADPARM;

	if (ctx->image.palette != NULL && 
			ctx->image.colour_type != COLOUR_TYPE_GREY)
		ctx->alloc(ctx->image.palette, 0, ctx->pw);

	if (ctx->image.data != NULL)
		ctx->alloc(ctx->image.data, 0, ctx->pw);

	if (ctx->image.scanline_idx != NULL)
		ctx->alloc(ctx->image.scanline_idx, 0, ctx->pw);

	if (ctx->chunk.data != NULL)
		ctx->alloc(ctx->chunk.data, 0, ctx->pw);

	if (ctx->src_scanline != NULL)
		ctx->alloc(ctx->src_scanline, 0, ctx->pw);

	if (ctx->dst_scanline != NULL)
		ctx->alloc(ctx->dst_scanline, 0, ctx->pw);

	inflateEnd(&ctx->zlib_stream);

	ctx->alloc(ctx, 0, ctx->pw);

	return NSPNG_OK;
}

nspng_error nspng_process_data(nspng_ctx *ctx, const uint8_t *data, size_t len)
{
	nspng_error error = NSPNG_OK;
	uint32_t old_amount;

	if (ctx == NULL || data == NULL)
		return NSPNG_BADPARM;

	while (len > 0 && error == NSPNG_OK) {
		if (ctx->state == STATE_START) {
			if (png_sig[ctx->id_idx] != *data) {
				error = NSPNG_INVALID;
			} else if (++ctx->id_idx == sizeof(png_sig)) {
				ctx->id_idx = 0;
				ctx->state = STATE_VERIFIED_SIGNATURE;
			}

			data++;
			len--;
		} else if (ctx->state != STATE_HAD_IEND) {
			old_amount = ctx->chunk.amount_read;

			error = read_chunk(ctx, data, len);

			data += ctx->chunk.amount_read - old_amount;
			len -= ctx->chunk.amount_read - old_amount;

			if (error == NSPNG_OK) {
				error = process_chunk(ctx);
			}
		} else /*if (ctx->state == STATE_HAD_IEND)*/ {
			/* Consume all data -- it's after IEND */
			data += len;
			len -= len;
		}
	}

	return error;
}

nspng_error nspng_get_dimensions(nspng_ctx *ctx, uint32_t *width, 
		uint32_t *height)
{
	if (ctx == NULL || width == NULL || height == NULL)
		return NSPNG_BADPARM;

	if (ctx->state != STATE_HAD_IEND)
		return NSPNG_INVALID;

	*width = ctx->image.width;
	*height = ctx->image.height;

	return NSPNG_OK;
}

