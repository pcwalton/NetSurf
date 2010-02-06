#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum font_style {
	REGULAR = 0,
	ITALIC = 1,
	BOLD = 2,
	ITALIC_BOLD = 3
};

struct glyph_offset {
	int code;
	long offset;
};

const int HEADER_MAX = 10000;

int cp1252[256] = {
		0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,
		0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F,
		0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017,
		0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F,
		0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
		0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
		0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
		0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
		0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
		0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
		0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
		0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
		0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067,
		0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
		0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
		0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x007F,
		0x20AC, 0xFFFD, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021,
		0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, 0xFFFD, 0x017D, 0xFFFD,
		0xFFFD, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,
		0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0xFFFD, 0x017E, 0x0178,
		0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7,
		0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,
		0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7,
		0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF,
		0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7,
		0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF,
		0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7,
		0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF,
		0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7,
		0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF,
		0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7,
		0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00FE, 0x00FF};


// Convert a hex digit to a number.  e.g. 'A' --> 10
int get_hex_digit_value(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	else if (c >= 'A' && c <= 'F')
		return (10 + c - 'A');
	else {
		printf("Bad hex value\n");
		exit(EXIT_FAILURE);
	}
}


// Convert Unicode codepoint to an int.  e.g. "000F" --> 15
int unicode_to_int(char* text)
{
	int result = 0;
	int i;

	for (i = 0; i < 4; i++)
		result += get_hex_digit_value(text[i]) * pow(16, 3 - i);

	return result;
}


// Numercical comparison function for qsort() and bsearch()
int compare(const void *a, const void *b)
{
	return (*(int *)a - ((struct glyph_offset*)b)->code);
}


// Scan glyphs file for faults and populate table of glyph offsets
bool prescan_glyphs(FILE *glyphs_file, struct glyph_offset **offsets, int *n)
{
	int count, c;
	char code[5];
	code[4] = '\0';
	char p = '\0';
	int previous = -1;

	// Count glyphs in file
	*n = 0;
	c = fgetc(glyphs_file);
	while (c != EOF) {
		if (c == '+' && p == 'U') {
			*n = *n + 1;
		}
		p = c;
		c = fgetc(glyphs_file);
	}

	// Allocate space for offset table
	*offsets = (struct glyph_offset*)
			malloc(*n * sizeof(struct glyph_offset));
	if (*offsets == NULL) {
		printf("Couldn't allocate offsets memory.\n");
		return false;
	}

	// Check glyphs in the file are in the correct order, and record
	// offset to each glyph section
	rewind(glyphs_file);
	count = 0;
	p = '\0';
	c = fgetc(glyphs_file);
	while (c != EOF) {
		if (c == '+' && p == 'U') {
			// Record offset to glyph entry
			(*offsets)[count].offset = ftell(glyphs_file);
			fread(code, 1, 4, glyphs_file);
			if (previous >= unicode_to_int(code)) {
				printf("Wrong order at: %s\n", code);
				return false;
			}
			// Record which glyph it is
			(*offsets)[count].code = unicode_to_int(code);
			previous = unicode_to_int(code);
			count++;
		}
		p = c;
		c = fgetc(glyphs_file);
	}

	rewind(glyphs_file);

	return true;
}


// Get a particular glyph from the glyphs file. Return true unless problem
// encountered
bool get_glyph(FILE *glyphs_file, char glyph_data[16][8], int n, int codepoint,
		struct glyph_offset *offsets, enum font_style style)
{
	enum font_style output_style;
	char code[5];
	char buffer[16 * 45];
	code[4] = '\0';
	int c, i;
	int x, y;
	struct glyph_offset *glyph;
	int count, position, start_pos;

	bool italic = true;
	bool bold = true;
	bool italic_bold = true;

	glyph = (struct glyph_offset*)bsearch(&codepoint, offsets, n,
			sizeof(struct glyph_offset), compare);

	if (glyph == NULL) {
		// This codepoint isn't available in the glyph file.
		return false;
	}

	// Move to the required glyph section in glyphs file
	fseek(glyphs_file, glyph->offset, SEEK_SET);

	// Check we're at the right place
	fread(code, 1, 4, glyphs_file);
	if (unicode_to_int(code) != glyph->code) {
		printf("Error finding glyph %.4X, missmatches %s\n"
				"Probably broken offset table.\n",
				glyph->code, code);
		exit(EXIT_FAILURE);
	}

	// Pass over rest of line
	c = fgetc(glyphs_file);
	while (c != EOF) {
		if (c == '\n')
			break;
		c = fgetc(glyphs_file);
	}

	// Pass over separator line
	fread(buffer, 1, 54, glyphs_file);
	if (strncmp("- - - - - - - - - - - - - - - - - - - - - - - - - - -\n",
			buffer, 54) != 0) {
		printf("Glyph file formatting error around glyph U+%.4X\n",
				glyph->code);
		exit(EXIT_FAILURE);
	}

	// Read glyph data
	fread(buffer, 1, 16 * 45, glyphs_file);

	// Check glyph data formatting is valid and find which styles are
	// present
	count = 0;
	while (buffer[count] != '\n') {
		count++;
	}
	if (count != 11 && count != 22 && count != 33 && count != 44) {
		printf("Glyph data formatting problem at glyph U+%.4X, "
				"line 0\n"
				"Possibly trailing whitespace.\n",
				glyph->code);
		exit(EXIT_FAILURE);
	}

	position = count;
	for (i = 1; i < 16; i++) {
		if (buffer[position] != '\n') {
			printf("Glyph data formatting problem at glyph U+%.4X, "
					"line %i\n"
					"Possibly trailing whitespace.\n",
					glyph->code, i);
			exit(EXIT_FAILURE);
		}
		position += count + 1;
	}
	// TODO handle italic_bold available but not italic or bold
	if (count < 44)
		italic_bold = false;
	if (count < 33)
		bold = false;
	if (count < 22)
		italic = false;

	// Set output_style depending on required style and style availability
	switch (style) {
	case REGULAR:
		output_style = REGULAR;
		break;
	case ITALIC:
		if (italic) {
			output_style = ITALIC;
		} else {
			output_style = REGULAR;
			printf("U+%.4X - Subst. regular for italic\n",
					glyph->code);
		}
		break;
	case BOLD:
		if (bold) {
			output_style = BOLD;
		} else {
			output_style = REGULAR;
			printf("U+%.4X - Subst. regular for bold\n",
					glyph->code);
		}
		break;
	case ITALIC_BOLD:

		if (italic_bold) {
			output_style = ITALIC_BOLD;
		} else if (italic) {
			output_style = ITALIC;
			printf("U+%.4X - Subst. italic for italic bold\n",
					glyph->code);
		} else {
			output_style = REGULAR;
			printf("U+%.4X - Subst. regular for italic bold\n",
					glyph->code);
		}
		break;
	default:
		printf("Insane font style requested\n");
		exit(EXIT_FAILURE);
	}

	// Populate output glyph data array with glyph
	start_pos = 3 + output_style * (8 + 3);
	for (y = 0; y < 16; y++) {
		position = start_pos + (y * (count + 1));
		for (x = 0; x < 8; x++) {
			glyph_data[y][x] = buffer[position + x];
		}
	}

	rewind(glyphs_file);
	return true;
}


int main(int argc, char** argv)
{
	FILE *glyphs_file;
	FILE *r_file, *i_file, *b_file, *ib_file;
	char glyph_data[16][8];
	struct glyph_offset *offsets;
	int n, x, y, i;
	int c, p = '\0';
	char buffer[HEADER_MAX];

	// Open glyph data file
	if ((glyphs_file = fopen("BitmapFont", "r")) == NULL)
		return EXIT_FAILURE;

	// Scan glyphs file for problems and create table of pointers
	// to each glyph's section in file
	if (!prescan_glyphs(glyphs_file, &offsets, &n))
		return EXIT_FAILURE;

	/* Uncomment to print all available glyphs in the chosen style as text
	// Go through the entire unicode range and print the glyph, if available
	for (i = 0x0000; i <= 0xFFFF; i++) {
		if (get_glyph(glyphs_file, glyph_data, n, i, offsets,
				REGULAR)) {
			printf("U+%.4X\n", i);
			// Print glyph
			for (y = 0; y < 16; y++) {
				printf("   ");
				for (x = 0; x < 8; x++) {
					printf("%c", glyph_data[y][x]);
				}
				printf("\n");
			}
			printf("\n");
		}
	}
	*/

	// Export CP1252 glyphs for framebuffer NetSurf
	if ((r_file = fopen("r", "w")) == NULL)
		return EXIT_FAILURE;

	rewind(glyphs_file);
	fread(buffer, 1, HEADER_MAX, glyphs_file);
	i = 0;
	while (i < HEADER_MAX - 1 && (buffer[i] != '\n' ||
			(buffer[i+1] == '*' && buffer[i] == '\n'))) {
		i++;
	}
	buffer[i + 1] = '\0';
	fprintf(r_file, "/*\n%s */\n\n", buffer);

	fprintf(r_file, "/* Don't edit this file, it was generated from the "
			"plain text source data. */\n\n");

	fprintf(r_file, "#include \"desktop/plotters.h\"\n"
			"#include \"utils/utf8.h\"\n\n"
			"#include \"framebuffer/font_internal.h\"\n\n"
			"#define FONTDATAMAX 4096\n\n"
			"static const uint32_t "
			"fontdata_regular[FONTDATAMAX] = {\n");

	for (i = 0; i < 256; i++) {
		if (!get_glyph(glyphs_file, glyph_data, n, cp1252[i], offsets,
				REGULAR)) {
			printf("Glyph U+%.4x\n", cp1252[i]);
			return EXIT_FAILURE;
		}
		fprintf(r_file, "\t");
		for (y = 0; y < 16; y++) {
			c = 0;
			for (x = 0; x < 8; x++) {
				if (glyph_data[y][x] == '#') {
					c += 1 << (7 - x);
				}
			}
			if (y == 7)
				fprintf(r_file, "0x%.2X,\n\t", c);
			else
				fprintf(r_file, "0x%.2X, ", c);
		}
		fprintf(r_file, "\n");
	}

	fprintf(r_file, "};\n\n"
			"const struct fb_font_desc font_regular = {\n"
			"\t.name = \"NetSurf Regular\",\n"
			"\t.width = 8,\n"
			"\t.height = 16,\n"
			"\t.encoding = \"CP1252\",\n"
			"\t.data = fontdata_regular,\n"
			"};", buffer);

	free(offsets);
	fclose(glyphs_file);
	fclose(r_file);

	return EXIT_SUCCESS;
}
