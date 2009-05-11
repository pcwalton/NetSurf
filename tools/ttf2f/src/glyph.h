#ifndef _TTF2F_GLYPH_H_
#define _TTF2F_GLYPH_H_

struct outline {
	enum { TERMINATE, MOVE_TO, LINE_TO, CURVE } type;
	union {
		struct { int composite; } terminate;
		struct { int x:12, y:12; } move_to;
		struct { int x:12, y:12; } line_to;
		struct { int x1:12, y1:12;
			 int x2:12, y2:12;
			 int x3:12, y3:12; } curve;
	} data;

	struct outline *next;
};

struct composite {
	short code;
	short x;
	short y;
	struct composite *next;
};

struct glyph {
	unsigned int code;           /* glyph code */
	const char *name;            /* glyph name */
	int  xMin:12, yMin:12;
	int  xMax:12, yMax:12;       /* glyph control box */
	int lsb;                     /* left side bearing of glyph,
                                        relative to origin */
	int ttf_pathlen;             /* number of points in glyph */
	short width;                 /* advance width of glyph */
	struct outline *outline;     /* outline of glyph */
	struct composite *composite; /* list of composite inclusions */

	int done_intmetrics : 2,
	    done_outlines   : 2,
	    done_encoding   : 2;
};

#endif

