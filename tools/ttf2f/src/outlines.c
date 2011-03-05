#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __riscos__
#include "swis.h"
#endif

#include "fm.h"
#include "ft.h"
#include "glyph.h"
#include "outlines.h"
#include "utils.h"

static ttf2f_result write_chunk(FILE* file, unsigned int chunk_no, 
		ttf2f_ctx *ctx, size_t *cur_glyph, 
		unsigned int *out_chunk_size);
static ttf2f_result append_glyph(struct glyph *g, size_t idx, ttf2f_ctx *ctx,
		struct chunk **chunk, unsigned int *chunk_size);

/**
 * Write the font outlines to file
 *
 * \param savein     Location to save in
 * \param name       Font name
 * \param ctx        Conversion context
 */
ttf2f_result outlines_write(const char *savein, const char *name,
		ttf2f_ctx *ctx,
		void (*callback)(int progress))
{
	struct outlines_header header;
	int table_end_len, i;
	unsigned int current_chunk_offset;
	int chunk_table_entry;
	size_t cur_glyph = 0;
	unsigned int slots;
	FILE *output;
	char out[1024];

	/* Number of slots in chunk tables */
	slots = ctx->nglyphs + N_ELEMENTS(ctx->latin1tab) - ctx->nlatin1;

	/* length of name + \0 + "Outlines" + \0 */
	table_end_len = strlen(name) + 10;

	/* fill in file header */
	header.id = 'F' + ('O' << 8) + ('N' << 16) + ('T' << 24);
	header.bpp = 0;
	header.version = 8;
	header.flags = 1000; /* design size of converted font */
	header.x0 = ctx->metrics->bbox[0];
	header.y0 = ctx->metrics->bbox[1];
	header.X = ctx->metrics->bbox[2] - ctx->metrics->bbox[0];
	header.Y = ctx->metrics->bbox[3] - ctx->metrics->bbox[1];
	header.chunk_data.chunk_table_offset =
		sizeof(struct outlines_header) + ((table_end_len + 6) & ~3);
	header.chunk_data.nchunks = ((slots + 31) & ~31) / 32;
	header.chunk_data.num_scaffold = 1; /* no scaffold lines */
	header.chunk_data.scaffold_flags = OUTLINES_SCAFFOLD_16BIT |
					OUTLINES_SCAFFOLD_NON_ZERO_WINDING;
	header.chunk_data.reserved[0] = 0;
	header.chunk_data.reserved[1] = 0;
	header.chunk_data.reserved[2] = 0;
	header.chunk_data.reserved[3] = 0;
	header.chunk_data.reserved[4] = 0;

	snprintf(out, 1024, "%s" DIR_SEP "Outlines" XXX_EXT, savein);
	if ((output = fopen(out, "wb") ) == NULL)
		return TTF2F_RESULT_OPEN;
	
	/* write file header */
	if (fwrite(&header, sizeof(struct outlines_header), 1, output)
		!= 1) goto error_write;

	/* write scaffold table */
	if (fputc(0x3, output) == EOF) goto error_write;
	if (fputc(0x0, output) == EOF) goto error_write;
	if (fputc(0x0, output) == EOF) goto error_write;

	/* table end */
	if (fprintf(output, "%s", name) < 0) goto error_write;
	if (fputc(0x0, output) == EOF) goto error_write;
	if (fprintf(output, "Outlines") < 0) goto error_write;
	if (fputc(0x0, output) == EOF) goto error_write;

	/* word align */
	i = table_end_len + 3 + sizeof(struct outlines_header);
	while (i++ < header.chunk_data.chunk_table_offset) {
		if (fputc(0x0, output) == EOF)
			goto error_write;
	}

	/* write chunk table */
	current_chunk_offset = header.chunk_data.chunk_table_offset +
				header.chunk_data.nchunks * 4 + 4;

	/* initialise chunk table */
	for (i = 0; i <= header.chunk_data.nchunks; i++) {
		if (fwrite(&current_chunk_offset, sizeof(int), 1, output) != 1)
			goto error_write;
	}

	/* write copyright information to file */
	if (fputc(0x0, output) == EOF) goto error_write;
	if (fprintf(output,
		"\n\n\n%s is %s\nConverted to RISC OS by TTF2F\n\n\n",
		ctx->metrics->name_full, 
		ctx->metrics->name_copyright) < 0) goto error_write;
	if (fputc(0x0, output) == EOF) goto error_write;
	current_chunk_offset += 42 + strlen(ctx->metrics->name_full) +
				strlen(ctx->metrics->name_copyright);

	/* Word align */
	while (current_chunk_offset % 4) {
		if (fputc(0x0, output) == EOF) goto error_write;
		current_chunk_offset++;
	}

	for (chunk_table_entry = 0; 
			chunk_table_entry != header.chunk_data.nchunks; 
			chunk_table_entry++) {
		unsigned int chunk_size;
		ttf2f_result err;

		callback((chunk_table_entry * 100) / ((slots + 31) + ~31) / 32);
		ttf2f_poll(1);

		/* Write chunk offset */
		fseek(output, header.chunk_data.chunk_table_offset +
				chunk_table_entry * 4, SEEK_SET);
		if (fwrite(&current_chunk_offset, sizeof(int), 1, output) != 1)
			goto error_write;

		/* seek to start of current chunk */
		fseek(output, current_chunk_offset, SEEK_SET);

		/* write chunk */
		err = write_chunk(output, chunk_table_entry, ctx, 
				&cur_glyph, &chunk_size);

		if (err != TTF2F_RESULT_OK) {
			fclose(output);
			return err;
		}

		current_chunk_offset += chunk_size;

		/* align to word boundary */
		while (current_chunk_offset % 4) {
			if (fputc(0x0, output) == EOF) goto error_write;
			current_chunk_offset++;
		}
	}

	/* Finally, write offset to end of file */
	fseek(output, header.chunk_data.chunk_table_offset +
			chunk_table_entry * 4, SEEK_SET);
	if (fwrite(&current_chunk_offset, sizeof(int), 1, output) != 1)
		goto error_write;

	fclose(output);

#ifdef __riscos__
	/* set type */
	_swix(OS_File, _INR(0,2), 18, out, 0xFF6);
#endif
	return TTF2F_RESULT_OK;

error_write:
	fclose(output);
	return TTF2F_RESULT_WRITE;
}

/**
 * Write chunk to outlines file
 *
 * \param file       Stream handle
 * \param chunk_no   The current chunk number (0..nchunks-1)
 * \param ctx        Conversion context
 * \param cur_glyph  Current glyph index
 * \return Size of this chunk, or 0 on failure
 */
ttf2f_result write_chunk(FILE* file, unsigned int chunk_no, ttf2f_ctx *ctx,
		size_t *cur_glyph, unsigned int *out_chunk_size)
{
	ttf2f_result err;
	struct glyph *g;
	struct chunk *chunk;
	unsigned int chunk_size;

	*out_chunk_size = 0;

	/* create chunk */
	chunk = calloc(1, sizeof(struct chunk));
	if (chunk == NULL)
		return TTF2F_RESULT_NOMEM;

	chunk->flags = 0x80000000;

	chunk_size = sizeof(struct chunk);

	if (chunk_no < N_ELEMENTS(ctx->latin1tab) / 32) {
		size_t i;

		/* Latin 1 table */
		for (i = 0; i != 32; i++) {
			g = ctx->latin1tab[(chunk_no * 32) + i];
			/* Skip undefined glyphs */
			if (g == NULL)
				continue;

			err = append_glyph(g, i, ctx, &chunk, &chunk_size);
			if (err != TTF2F_RESULT_OK) {
				free(chunk);
				return err;
			}

			g->done_outlines = 1;
		}
	} else {
		size_t nchars = 0;
		size_t idx = *cur_glyph;

		/* Everything else */
		while (nchars < 32) {
			/* Done if there are no more glyphs */
			if (idx == ctx->nglyphs)
				break;

			g = &ctx->glyphs[idx++];

			/* Skip glyphs already written */
			if (g->done_outlines)
				continue;

			err = append_glyph(g, nchars, ctx, &chunk, &chunk_size);
			if (err != TTF2F_RESULT_OK) {
				free(chunk);
				return err;
			}

			g->done_outlines = 1;

			nchars++;
		}

		*cur_glyph = idx;
	}

	/* write chunk to file */
	if (fwrite(chunk, chunk_size, 1, file) != 1) {
		free(chunk);
		return TTF2F_RESULT_WRITE;
	}

	free(chunk);

	*out_chunk_size = chunk_size;

	return TTF2F_RESULT_OK;
}

ttf2f_result append_glyph(struct glyph *g, size_t idx, ttf2f_ctx *ctx,
		struct chunk **chunk, unsigned int *chunk_size)
{
	struct chunk *temp;
	struct outline *o, *next;
	struct char_data *character;
	size_t outline_size = 0;
	char *outline;

	ttf2f_poll(1);

	/* no path => skip character unless space */
	if (g->ttf_pathlen == 0 && g->code != 0x0020) {
		(*chunk)->offset[idx] = 0;
		return TTF2F_RESULT_OK;
	}

	/* offset from index start */
	(*chunk)->offset[idx] = (*chunk_size) - 4;

	temp = realloc((*chunk), (*chunk_size) + sizeof(struct char_data));
	if (temp == NULL)
		return TTF2F_RESULT_NOMEM;
	(*chunk) = temp;

	character = (void *)((char *) (*chunk) + (*chunk_size));

	(*chunk_size) += sizeof(struct char_data);

	character->flags = CHAR_12BIT_COORDS | CHAR_OUTLINE;
	/* character x0, y0 */
	character->x0y0[0] = g->xMin & 0xFF;
	character->x0y0[1] = ((g->yMin << 4) & 0xF0) | ((g->xMin >> 8) & 0xF);
	character->x0y0[2] = (g->yMin >> 4) & 0xFF;
	/* character width, height */
	character->xsys[0] = (g->xMax - g->xMin) & 0xFF;
	character->xsys[1] = (((g->yMax - g->yMin) << 4) & 0xF) |
				(((g->xMax - g->xMin) >> 8) & 0xF);
	character->xsys[2] = ((g->yMax - g->yMin) >> 4) & 0xFF;

	/* Nasty hack for space character */
	if (g->ttf_pathlen == 0) {
		assert(g->code == 0x0020);

		temp = realloc((*chunk), (*chunk_size) + 1);
		if (temp == NULL)
			return TTF2F_RESULT_NOMEM;
		(*chunk) = temp;

		outline = (char *) (*chunk) + (*chunk_size);

		(*chunk_size) += 1;

		/* Just terminate path */
		(*outline) = 0;

		return TTF2F_RESULT_OK;
	}

	/* decompose glyph path */
	glpath(ctx, g - ctx->glyphs);

	/* Step 1: count size of outline data */
	for (o = g->outline; o != NULL; o = o->next) {
		/* movement type */
		switch (o->type) {
		case TERMINATE:
			/* end of outline */
			outline_size += 1;
			break;
		case MOVE_TO:
			outline_size += 4;
			break;
		case LINE_TO:
			outline_size += 4;
			break;
		case CURVE:
			outline_size += 10;
			break;
		}
	}

	temp = realloc((*chunk), (*chunk_size) + outline_size);
	if (temp == NULL)
		return TTF2F_RESULT_NOMEM;
	(*chunk) = temp;

	outline = (char *) (*chunk) + (*chunk_size);

	(*chunk_size) += outline_size;

	/* Step 2: populate outline data */
	for (o = g->outline; o != NULL; o = next) {
		switch (o->type) {
		case TERMINATE:
			*(outline++) = 0;
			break;
		case MOVE_TO:
			/* move to point */
			/* id, no scaffold */
			*(outline++) = 1;
			/* x, y */
			*(outline++) = o->data.move_to.x & 0xFF;
			*(outline++) = (((o->data.move_to.y << 4) & 0xF0) | 
					((o->data.move_to.x >> 8) & 0xF));
			*(outline++) = (o->data.move_to.y >> 4) & 0xFF;
			break;
		case LINE_TO:
			/* draw line to point */
			/* id, no scaffold */
			*(outline++) = 2;
			/* x, y */
			*(outline++) = o->data.line_to.x & 0xFF;
			*(outline++) = (((o->data.line_to.y << 4) & 0xF0) | 
					((o->data.line_to.x >> 8) & 0xF));
			*(outline++) = (o->data.line_to.y >> 4) & 0xFF;
			break;
		case CURVE:
			/* draw bezier curve to point */
			/* id, no scaffold */
			*(outline++) = 3;
			/* x1, y1 */
			*(outline++) = o->data.curve.x1 & 0xFF;
			*(outline++) = (((o->data.curve.y1 << 4) & 0xF0) | 
					((o->data.curve.x1 >> 8) & 0xF));
			*(outline++) = (o->data.curve.y1 >> 4) & 0xFF;
			/* x2, y2 */
			*(outline++) = o->data.curve.x2 & 0xFF;
			*(outline++) = (((o->data.curve.y2 << 4) & 0xF0) | 
					((o->data.curve.x2 >> 8) & 0xF));
			*(outline++) = (o->data.curve.y2 >> 4) & 0xFF;
			/* x3, y3 */
			*(outline++) = o->data.curve.x3 & 0xFF;
			*(outline++) = (((o->data.curve.y3 << 4) & 0xF0) | 
					((o->data.curve.x3 >> 8) & 0xF));
			*(outline++) = (o->data.curve.y3 >> 4) & 0xFF;
			break;
		}

		next = o->next;
		free(o);
	}

	return TTF2F_RESULT_OK;
}

