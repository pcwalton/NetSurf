#ifndef _TTF2F_FM_H_
#define _TTF2F_FM_H_

struct font_metrics {
	/* post */
	double italic_angle;
	short underline_position;
	short underline_thickness;
	short is_fixed_pitch;

	/* hhea */
	short ascender; 
	short descender;

	/* head */
	unsigned short units_per_em;
	short bbox[4];

	/* name */
	char *name_copyright;
	const char *name_family;
	const char *name_style;
	char *name_full;
	char *name_version;
	char *name_ps;

	/* other */
	int force_bold;
};

#endif
