/*
 * This file is part of LibNSPNG.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef libnspng_h_
#define libnspng_h_

#include <stddef.h>
#include <stdint.h>

typedef enum nspng_error {
	NSPNG_OK = 0,
	NSPNG_NOMEM = 1,
	NSPNG_NEEDDATA = 2,
	NSPNG_BADPARM = 3,
	NSPNG_INVALID = 4
} nspng_error;

typedef struct nspng_rect {
	uint32_t x0; /* Left (inclusive) */
	uint32_t y0; /* Top (inclusive) */
	uint32_t x1; /* Right (exclusive) */
	uint32_t y1; /* Bottom (exclusive) */
} nspng_rect;

typedef nspng_error (*nspng_row_callback)(const uint8_t *row, 
		uint32_t rowbytes, uint32_t rownum, int pass, void *pw);

typedef void *(*nspng_allocator_fn)(void *ptr, size_t len, void *pw);

typedef struct nspng_ctx nspng_ctx;

nspng_error nspng_ctx_create(nspng_allocator_fn alloc, void *pw, 
		nspng_ctx **result);
nspng_error nspng_ctx_destroy(nspng_ctx *ctx);

nspng_error nspng_process_data(nspng_ctx *ctx, const uint8_t *data, size_t len);

nspng_error nspng_get_dimensions(nspng_ctx *ctx, uint32_t *width, 
		uint32_t *height);

nspng_error nspng_render(nspng_ctx *ctx, const nspng_rect *clip,
		nspng_row_callback callback, void *pw);

#endif
