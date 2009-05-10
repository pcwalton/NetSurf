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

ttf2f_result write_chunk(FILE* file, int chunk_no, ttf2f_ctx *ctx,
		unsigned int *out_chunk_size);

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
	unsigned int chunk_table_entry;
	FILE *output;
	char out[1024];

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
	header.chunk_data.nchunks = (ctx->nglyphs / 32) + 2;
	header.chunk_data.num_scaffold = 1; /* no scaffold lines */
	header.chunk_data.scaffold_flags = OUTLINES_SCAFFOLD_16BIT |
					OUTLINES_SCAFFOLD_NON_ZERO_WINDING;
	header.chunk_data.reserved[0] = 0;
	header.chunk_data.reserved[1] = 0;
	header.chunk_data.reserved[2] = 0;
	header.chunk_data.reserved[3] = 0;
	header.chunk_data.reserved[4] = 0;

	snprintf(out, 1024, "%s" DIR_SEP "Outlines", savein);
	if ((output = fopen(out, "wb+") ) == NULL)
		return TTF2F_RESULT_OPEN;
	
	/* write file header */
	if (fwrite((void*)&header, sizeof(struct outlines_header), 1, output)
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
	while (i++ < header.chunk_data.chunk_table_offset)
		if (fputc(0x0, output) == EOF) goto error_write;

	/* write chunk table */
	chunk_table_entry = 1;
	current_chunk_offset = header.chunk_data.chunk_table_offset +
				header.chunk_data.nchunks * 4 + 4;

	/* initialise chunk table */
	for (i = 0; i <= header.chunk_data.nchunks; i++) {
		if (fwrite((void*)&current_chunk_offset, sizeof(int),
			1, output) != 1) goto error_write;
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

	while(current_chunk_offset % 4) {
		if (fputc(0x0, output) == EOF) goto error_write;
		current_chunk_offset++;
	}

	/* fill in offsets 0 and 1 */
	fseek(output, header.chunk_data.chunk_table_offset, SEEK_SET);
	if (fwrite((void*)&current_chunk_offset, sizeof(int), 1, output) != 1)
		goto error_write;

	if (fwrite((void*)&current_chunk_offset, sizeof(int), 1, output) != 1)
		goto error_write;

	for (; header.chunk_data.nchunks > 1; header.chunk_data.nchunks--) {
		unsigned int chunk_size;
		ttf2f_result err;

		callback((chunk_table_entry * 100) / ((ctx->nglyphs / 32) + 2));
		ttf2f_poll(1);

		/* seek to start of current chunk */
		fseek(output, current_chunk_offset, SEEK_SET);

		/* write chunk */
		err = write_chunk(output, chunk_table_entry - 1, ctx, 
				&chunk_size);

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

		/* fill in next chunk table entry */
		fseek(output, header.chunk_data.chunk_table_offset +
				(chunk_table_entry+1) * 4, SEEK_SET);
		if (fwrite((void*)&current_chunk_offset, sizeof(int), 1,
			output) != 1) goto error_write;

		chunk_table_entry++;
	}

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
 * \return Size of this chunk, or 0 on failure
 */
ttf2f_result write_chunk(FILE* file, int chunk_no, ttf2f_ctx *ctx,
		unsigned int *out_chunk_size)
{
	const struct glyph *g;
	struct chunk *chunk;
	unsigned int chunk_size;
	struct outline *o, *next;
	struct char_data *character;
	size_t i;

	*out_chunk_size = 0;

	/* create chunk */
	chunk = calloc(1, sizeof(struct chunk));
	if (chunk == NULL)
		return TTF2F_RESULT_NOMEM;

	chunk->flags = 0x80000000;

	chunk_size = sizeof(struct chunk);

	/* 32 chars in each chunk */
	for (i = 0; i != 32; i++) {

		ttf2f_poll(1);

		if ((chunk_no * 32) + i >= ctx->nglyphs)
			/* exit if we've reached the end of the input */
			break;

		/* get glyph */
		g = &ctx->glyphs[(chunk_no * 32) + i];

		/* no path => skip character */
		if (g->ttf_pathlen == 0) {
			chunk->offset[i] = 0;
			continue;
		}

		/* offset from index start */
		chunk->offset[i] = chunk_size - 4;

		chunk = realloc((char*)chunk,
				chunk_size+sizeof(struct char_data));
		if (chunk == NULL)
			return TTF2F_RESULT_NOMEM;

		character = (struct char_data*)(void*)((char*)chunk + chunk_size);

		chunk_size += sizeof(struct char_data);

		character->flags = CHAR_12BIT_COORDS | CHAR_OUTLINE;
		/* character x0, y0 */
		character->x0y0[0] = g->xMin & 0xFF;
		character->x0y0[1] = ((g->yMin << 4) & 0xF0) |
						((g->xMin >> 8) & 0xF);
		character->x0y0[2] = (g->yMin >> 4) & 0xFF;
		/* character width, height */
		character->xsys[0] = (g->xMax - g->xMin) & 0xFF;
		character->xsys[1] = (((g->yMax - g->yMin) << 4) & 0xF) |
					(((g->xMax - g->xMin) >> 8) & 0xF);
		character->xsys[2] = ((g->yMax - g->yMin) >> 4) & 0xFF;

		/* decompose glyph path */
		glpath(ctx->face, (chunk_no * 32) + i, ctx->glyphs);

		for (o = g->outline; o; o = next) {
			if (!o)
				break;

			/* movement type */
			switch (o->type) {
			case TERMINATE:
				/* end of outline */
				chunk = realloc((char*)chunk, chunk_size + 1);
				if (chunk == NULL)
					return TTF2F_RESULT_NOMEM;
				*((char*)chunk+chunk_size-1) = 0;
				chunk_size += 1;
				break;
			case MOVE_TO:
				/* move to point */
				chunk = realloc((char*)chunk, chunk_size + 4);
				if (chunk == NULL)
					return TTF2F_RESULT_NOMEM;
				/* id, no scaffold */
				*((char*)chunk+chunk_size-1) = 1;
				/* x, y */
				*((char*)chunk+chunk_size) = 
						o->data.move_to.x & 0xFF;
				*((char*)chunk+chunk_size+1) = 
					(((o->data.move_to.y << 4) & 0xF0) | 
					((o->data.move_to.x >> 8) & 0xF));
				*((char*)chunk+chunk_size+2) = 
					(o->data.move_to.y >> 4) & 0xFF;
				chunk_size += 4;
				break;
			case LINE_TO:
				/* draw line to point */
				chunk = realloc((char*)chunk, chunk_size + 4);
				if (chunk == NULL)
					return TTF2F_RESULT_NOMEM;
				/* id, no scaffold */
				*((char*)chunk+chunk_size-1) = 2;
				/* x, y */
				*((char*)chunk+chunk_size) = 
						o->data.line_to.x & 0xFF;
				*((char*)chunk+chunk_size+1) = 
					(((o->data.line_to.y << 4) & 0xF0) | 
					((o->data.line_to.x >> 8) & 0xF));
				*((char*)chunk+chunk_size+2) = 
					(o->data.line_to.y >> 4) & 0xFF;
				chunk_size += 4;
				break;
			case CURVE:
				/* draw bezier curve to point */
				chunk = realloc((char*)chunk, chunk_size + 10);
				if (chunk == NULL)
					return TTF2F_RESULT_NOMEM;
				/* id, no scaffold */
				*((char*)chunk+chunk_size-1) = 3;
				/* x1, y1 */
				*((char*)chunk+chunk_size) = 
						o->data.curve.x1 & 0xFF;
				*((char*)chunk+chunk_size+1) = 
					(((o->data.curve.y1 << 4) & 0xF0) | 
					((o->data.curve.x1 >> 8) & 0xF));
				*((char*)chunk+chunk_size+2) = 
					(o->data.curve.y1 >> 4) & 0xFF;
				/* x2, y2 */
				*((char*)chunk+chunk_size+3) = 
					o->data.curve.x2 & 0xFF;
				*((char*)chunk+chunk_size+4) = 
					(((o->data.curve.y2 << 4) & 0xF0) | 
					((o->data.curve.x2 >> 8) & 0xF));
				*((char*)chunk+chunk_size+5) = 
					(o->data.curve.y2 >> 4) & 0xFF;
				/* x3, y3 */
				*((char*)chunk+chunk_size+6) = 
					o->data.curve.x3 & 0xFF;
				*((char*)chunk+chunk_size+7) = 
					(((o->data.curve.y3 << 4) & 0xF0) | 
					((o->data.curve.x3 >> 8) & 0xF));
				*((char*)chunk+chunk_size+8) = 
					(o->data.curve.y3 >> 4) & 0xFF;
				chunk_size += 10;
				break;
			}

			next = o->next;
			free(o);
		}

		/* shift chunk end pointer to end of character */
		chunk_size -= 1;
	}

	/* write chunk to file */
	if (fwrite((void*)chunk, chunk_size, 1, file) != 1) {
		free(chunk);
		return TTF2F_RESULT_WRITE;
	}

	free(chunk);

	*out_chunk_size = chunk_size;

	return TTF2F_RESULT_OK;
}

