#ifndef _TTF2F_OUTLINES_H_
#define _TTF2F_OUTLINES_H_

#include "context.h"
#include "utils.h"

struct chunk_data {
	int chunk_table_offset;
	int nchunks;
	int num_scaffold;
	int scaffold_flags;
	int reserved[5];
};

struct outlines_header {
	int   id;
	char  bpp;
	char  version;
	short flags;
	short x0;
	short y0;
	short X;
	short Y;
	struct chunk_data chunk_data;
};

#define OUTLINES_SCAFFOLD_16BIT            0x1
#define OUTLINES_SCAFFOLD_NO_AA            0x2
#define OUTLINES_SCAFFOLD_NON_ZERO_WINDING 0x4
#define OUTLINES_SCAFFOLD_BIG_TABLE        0x8

struct chunk {
	unsigned int  flags;
	unsigned int  offset[32];
};

struct char_data {
	char  flags;
	char  x0y0[3];
	char  xsys[3];
} __attribute__((packed));

#define CHAR_12BIT_COORDS 0x01
#define CHAR_1BPP         0x02
#define CHAR_BLACK        0x04
#define CHAR_OUTLINE      0x08
#define CHAR_COMPOSITE    0x10
#define CHAR_HAS_ACCENT   0x20
#define CHAR_16BIT_ASCII  0x40
#define CHAR_RESERVED     0x80

struct glyph;
struct font_metrics;

ttf2f_result outlines_write(const char *savein, 
		const char *name, ttf2f_ctx *ctx,
		void (*callback)(int progress));

#endif
