#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "swis.h"

#include "fm.h"
#include "ft.h"
#include "glyph.h"
#include "outlines.h"
#include "utils.h"

static unsigned int write_chunk(FILE* file, int chunk_no,
		struct glyph *glyph_list, int list_size);

/**
 * Write the font outlines to file
 *
 * \param savein     Location to save in
 * \param name       Font name
 * \param glyph_list List of all glyphs in font
 * \param list_size  Size of glyph list
 * \param metrics    Global font metrics
 */
void write_outlines(const char *savein, const char *name,
		struct glyph *glyph_list, int list_size,
		struct font_metrics *metrics,
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
	header.x0 = metrics->bbox[0];
	header.y0 = metrics->bbox[1];
	header.X = metrics->bbox[2] - metrics->bbox[0];
	header.Y = metrics->bbox[3] - metrics->bbox[1];
	header.chunk_data.chunk_table_offset =
		sizeof(struct outlines_header) + ((table_end_len + 6) & ~3);
	header.chunk_data.nchunks = (list_size / 32) + 2;
	header.chunk_data.num_scaffold = 1; /* no scaffold lines */
	header.chunk_data.scaffold_flags = OUTLINES_SCAFFOLD_16BIT |
					OUTLINES_SCAFFOLD_NON_ZERO_WINDING;
	header.chunk_data.reserved[0] = 0;
	header.chunk_data.reserved[1] = 0;
	header.chunk_data.reserved[2] = 0;
	header.chunk_data.reserved[3] = 0;
	header.chunk_data.reserved[4] = 0;

	snprintf(out, 1024, "%s.Outlines", savein);
	output = fopen(out, "wb+");
	/* write file header */
	fwrite((void*)&header, sizeof(struct outlines_header), 1, output);
	/* write scaffold table */
	fputc(0x3, output);
	fputc(0x0, output);
	fputc(0x0, output);
	/* table end */
	fprintf(output, "%s", name);
	fputc(0x0, output);
	fprintf(output, "Outlines");
	fputc(0x0, output);
	/* word align */
	i = table_end_len + 3 + sizeof(struct outlines_header);
	while (i++ < header.chunk_data.chunk_table_offset)
		fputc(0x0, output);

	/* write chunk table */
	chunk_table_entry = 1;
	current_chunk_offset = header.chunk_data.chunk_table_offset +
				header.chunk_data.nchunks * 4 + 4;

	/* initialise chunk table */
	for (i = 0; i <= header.chunk_data.nchunks; i++) {
		fwrite((void*)&current_chunk_offset, sizeof(int),
			1, output);
	}

	/* write copyright information to file */
	fputc(0x0, output);
	fprintf(output,
		"\n\n\n%s is %s\nConverted to RISC OS by TTF2F\n\n\n",
		metrics->name_full, metrics->name_copyright);
	fputc(0x0, output);
	current_chunk_offset += 42 + strlen(metrics->name_full) +
				strlen(metrics->name_copyright);

	while(current_chunk_offset % 4) {
		fputc(0x0, output);
		current_chunk_offset++;
	}

	/* fill in offsets 0 and 1 */
	fseek(output, header.chunk_data.chunk_table_offset, SEEK_SET);
	fwrite((void*)&current_chunk_offset, sizeof(int), 1, output);
	fwrite((void*)&current_chunk_offset, sizeof(int), 1, output);

	for (; header.chunk_data.nchunks > 1; header.chunk_data.nchunks--) {

		callback((chunk_table_entry * 100) / ((list_size / 32) + 2));
		ttf2f_poll(1);

		/* seek to start of current chunk */
		fseek(output, current_chunk_offset, SEEK_SET);

		/* write chunk */
		current_chunk_offset += write_chunk(output,
				chunk_table_entry - 1, glyph_list,
				list_size);

		/* align to word boundary */
		while (current_chunk_offset % 4) {
			fputc(0x0, output);
			current_chunk_offset++;
		}

		/* fill in next chunk table entry */
		fseek(output, header.chunk_data.chunk_table_offset +
				(chunk_table_entry+1) * 4, SEEK_SET);
		fwrite((void*)&current_chunk_offset, sizeof(int), 1,
			output);

		chunk_table_entry++;
	}

	fclose(output);

	/* set type */
	_swix(OS_File, _INR(0,2), 18, out, 0xFF6);
}

/**
 * Write chunk to outlines file
 *
 * \param file       Stream handle
 * \param chunk_no   The current chunk number (0..nchunks-1)
 * \param glyph_list List of all glyphs in font
 * \param list_size  Size of glyph list
 * \return Size of this chunk, or 0 on failure
 */
unsigned int write_chunk(FILE* file, int chunk_no, struct glyph *glyph_list,
		int list_size)
{
	struct glyph *g;
	struct chunk *chunk;
	unsigned int chunk_size;
	struct outline *o, *next;
	struct char_data *character;
	int i;

	/* create chunk */
	chunk = calloc(1, sizeof(struct chunk));
	if (!chunk)
		return 0;

	chunk->flags = 0x80000000;

	chunk_size = sizeof(struct chunk);

	/* 32 chars in each chunk */
	for (i = 0; i != 32; i++) {

		ttf2f_poll(1);

		if ((chunk_no * 32) + i >= list_size)
			/* exit if we've reached the end of the input */
			break;

		/* get glyph */
		g = &glyph_list[(chunk_no * 32) + i];

		/* no path => skip character */
		if (g->ttf_pathlen == 0) {
			chunk->offset[i] = 0;
			continue;
		}

		/* offset from index start */
		chunk->offset[i] = chunk_size - 4;

		chunk = realloc((char*)chunk,
				chunk_size+sizeof(struct char_data));
		if (!chunk)
			return 0;
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
		glpath((chunk_no * 32) + i, glyph_list);

		for (o = g->outline; o; o = next) {
			if (!o)
				break;

			/* movement type */
			switch (o->type) {
				case TERMINATE:
					/* end of outline */
					chunk = realloc((char*)chunk,
							chunk_size+1);
					if (!chunk)
						return 0;
					*((char*)chunk+chunk_size-1) = 0;
					chunk_size += 1;
					break;
				case MOVE_TO:
					/* move to point */
					chunk = realloc((char*)chunk,
							chunk_size+4);
					if (!chunk)
						return 0;
					/* id, no scaffold */
					*((char*)chunk+chunk_size-1) = 1;
					/* x, y */
					*((char*)chunk+chunk_size) = o->data.move_to.x & 0xFF;
					*((char*)chunk+chunk_size+1) = (((o->data.move_to.y << 4) & 0xF0) | ((o->data.move_to.x >> 8) & 0xF));
					*((char*)chunk+chunk_size+2) = (o->data.move_to.y >> 4) & 0xFF;
					chunk_size += 4;
					break;
				case LINE_TO:
					/* draw line to point */
					chunk = realloc((char*)chunk,
							chunk_size+4);
					if (!chunk)
						return 0;
					/* id, no scaffold */
					*((char*)chunk+chunk_size-1) = 2;
					/* x, y */
					*((char*)chunk+chunk_size) = o->data.line_to.x & 0xFF;
					*((char*)chunk+chunk_size+1) = (((o->data.line_to.y << 4) & 0xF0) | ((o->data.line_to.x >> 8) & 0xF));
					*((char*)chunk+chunk_size+2) = (o->data.line_to.y >> 4) & 0xFF;
					chunk_size += 4;
					break;
				case CURVE:
					/* draw bezier curve to point */
					chunk = realloc((char*)chunk,
							chunk_size+10);
					if (!chunk)
						return 0;
					/* id, no scaffold */
					*((char*)chunk+chunk_size-1) = 3;
					/* x1, y1 */
					*((char*)chunk+chunk_size) = o->data.curve.x1 & 0xFF;
					*((char*)chunk+chunk_size+1) = (((o->data.curve.y1 << 4) & 0xF0) | ((o->data.curve.x1 >> 8) & 0xF));
					*((char*)chunk+chunk_size+2) = (o->data.curve.y1 >> 4) & 0xFF;
					/* x2, y2 */
					*((char*)chunk+chunk_size+3) = o->data.curve.x2 & 0xFF;
					*((char*)chunk+chunk_size+4) = (((o->data.curve.y2 << 4) & 0xF0) | ((o->data.curve.x2 >> 8) & 0xF));
					*((char*)chunk+chunk_size+5) = (o->data.curve.y2 >> 4) & 0xFF;
					/* x3, y3 */
					*((char*)chunk+chunk_size+6) = o->data.curve.x3 & 0xFF;
					*((char*)chunk+chunk_size+7) = (((o->data.curve.y3 << 4) & 0xF0) | ((o->data.curve.x3 >> 8) & 0xF));
					*((char*)chunk+chunk_size+8) = (o->data.curve.y3 >> 4) & 0xFF;
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
	fwrite((void*)chunk, chunk_size, 1, file);

	free(chunk);

	return chunk_size;
}

