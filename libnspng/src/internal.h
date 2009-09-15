/*
 * This file is part of LibNSPNG.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef nspng_internal_h_
#define nspng_internal_h_

#include <zlib.h>

#include <stdint.h>

#include <libnspng.h>

typedef enum nspng_state {
	STATE_START,
	STATE_VERIFIED_SIGNATURE,
	STATE_HAD_IHDR,
	STATE_HAD_PLTE,
	STATE_HAD_IDAT,
	STATE_HAD_IEND
} nspng_state;

typedef enum nspng_chunk_state {
	CHUNK_STATE_START,
	CHUNK_STATE_HAD_LENGTH,
	CHUNK_STATE_HAD_TYPE,
	CHUNK_STATE_HAD_DATA,
	CHUNK_STATE_HAD_CRC
} nspng_chunk_state;

typedef enum nspng_colour_type {
	COLOUR_BITS_PALETTED = 0x1,
	COLOUR_BITS_TRUE     = 0x2,
	COLOUR_BITS_ALPHA    = 0x4,

	COLOUR_TYPE_GREY     = 0,
	COLOUR_TYPE_RGB      = COLOUR_BITS_TRUE,
	COLOUR_TYPE_INDEXED  = COLOUR_BITS_TRUE | COLOUR_BITS_PALETTED,
	COLOUR_TYPE_GREY_A   = COLOUR_BITS_ALPHA,
	COLOUR_TYPE_RGBA     = COLOUR_BITS_TRUE | COLOUR_BITS_ALPHA
} nspng_colour_type;

typedef enum nspng_compression_type {
	COMPRESSION_TYPE_DEFLATE = 0
} nspng_compression_type;

typedef enum nspng_filter_method {
	FILTER_METHOD_ADAPTIVE = 0
} nspng_filter_method;

typedef enum nspng_adaptive_filter_type {
	ADAPTIVE_FILTER_NONE = 0,
	ADAPTIVE_FILTER_SUB = 1,
	ADAPTIVE_FILTER_UP = 2,
	ADAPTIVE_FILTER_AVERAGE = 3,
	ADAPTIVE_FILTER_PAETH = 4
} nspng_adaptive_filter_type;

typedef enum nspng_interlace_type {
	INTERLACE_TYPE_NONE = 0,
	INTERLACE_TYPE_ADAM7 = 1
} nspng_interlace_type;

typedef struct nspng_chunk {
	uint32_t length;
	uint32_t type;
	uint8_t *data;
	uint32_t crc;

	uint32_t amount_read; /* <= length + 12 */

	uint32_t data_alloc; /* Number of bytes allocated for data */

	uint32_t computed_crc; /* CRC computed from data */
} nspng_chunk;

typedef struct nspng_image {
	uint32_t width;
	uint32_t height;

	struct {
		uint32_t idx;
		uint32_t bps;
	} passes[7];

#define SCANLINE_COMPRESSED_FLAG (1u<<31)
#define SCANLINE_IS_COMPRESSED(image, idx) \
		((image)->scanline_idx[(idx)] & SCANLINE_COMPRESSED_FLAG)
#define SCANLINE_OFFSET(image, idx) \
		((image)->scanline_idx[(idx)] & ~SCANLINE_COMPRESSED_FLAG)
#define SCANLINE_LEN(image, idx) \
		(((idx) < (image)->n_scanlines - 1) \
			? ((SCANLINE_OFFSET((image), (idx) + 1)) - \
			   (SCANLINE_OFFSET((image), (idx)))) \
			: (image->data_len - SCANLINE_OFFSET((image), (idx))))

	uint32_t n_scanlines;
	uint32_t *scanline_idx;
	uint32_t bytes_per_scanline;
	uint32_t filtered_byte_offset;

	uint8_t *data;
	uint32_t data_len;
	uint32_t data_alloc;

	uint32_t palette_entries;
	uint32_t *palette;

	uint32_t gamma;

	struct {
		uint16_t r;
		uint16_t g;
		uint16_t b;
	} transparency;

	struct {
		uint16_t r;
		uint16_t g;
		uint16_t b;
	} background;

	uint32_t had_gama : 1,
		 had_trns : 1,
		 had_bkgd : 1;

	uint8_t bit_depth;
	uint8_t colour_type;
	uint8_t compression;
	uint8_t filter;
	uint8_t interlace;
	uint8_t bits_per_pixel;
} nspng_image;

struct nspng_ctx {
	nspng_state state;
	nspng_chunk_state chunk_state;

	uint8_t id_idx;

	uint32_t cur_scanline;
	uint32_t bytes_read_for_scanline;
	nspng_adaptive_filter_type scanline_filter;
	uint8_t prev_pixel[8];
	uint8_t *src_scanline;

	uint32_t *rowbuf;

	z_stream zlib_stream;

	nspng_chunk chunk;

	nspng_image image;

	nspng_allocator_fn alloc;
	void *pw;
};

#endif
