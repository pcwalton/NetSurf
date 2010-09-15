/*
 * Copyright 2009-2010 Michael Drake <tlsa@netsurf-browser.org>
 *
 * This file is part of AlphaGen.
 *
 * Licensed under the MIT License,
 * http://www.opensource.org/licenses/mit-license.php
 *
 * AlphaGen is a tool for obtaining a bitmap image with an alpha channel from
 * software which lacks this facility itself.  It recovers alpha channel
 * information from two input images; one with a black baground and one with a
 * white background.
 *
 * Inputs required:
 *     + The input image exported on a _black_ background.
 *     + The input image exported on a _white_ background.
 * Output:
 *     + The resultant image with an alpha channel.
 *
 * Note that both images must be the same size.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <png.h>


/* Bitmap image representation */
struct image {
	unsigned int width;
	unsigned int height;
	unsigned int channels;
	uint8_t **d; /* data */
};

/* Main functionality of AlphaGen. */
bool alphagen(char *black_name, char *white_name, char *alpha_name);
bool alphagen_check_inputs(struct image *b, struct image *w);

/* image_* functions, so the libpng stuff is kept hidden */
bool image_init(struct image *img, int width, int height, int channels);
void image_free(struct image *img);
bool image_read_png(char *file_name, struct image *img);
bool image_write_png(char *file_name, struct image *img);

/**
 * Main entry point from OS.
 */
int main(int argc, char** argv)
{
	/* Validate execution arguments. */
	if (argc != 4) {
		/* Too few arguments */
		printf("Usage: %s <black> <white> <output>\n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Generate the alpha channel bitmap. */
	if (!alphagen(argv[1], argv[2], argv[3])) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


/**
 * Performs alpha channel recovery on an image provided with black and white
 * backgrounds.
 *
 * \param  black_name  filename for black background input PNG
 * \param  white_name  filename for white background input PNG
 * \param  alpha_name  filename for output PNG
 * \return  true on success, false on error
 */
bool alphagen(char *black_name, char *white_name, char *alpha_name)
{
	struct image b, w, a;
	unsigned int row_data_width;
	unsigned int x, y, z;
	uint8_t ar, ag, ab;

	/* Default background colour; 0 = black, 255 = white */
	const uint8_t bg = 255;

	/* Load input PNGs */
	if (!image_read_png(black_name, &b)) {
		printf("Couldn't load \"%s\" as black background input.\n",
				black_name);
		return false;
	}
	if (!image_read_png(white_name, &w)) {
		printf("Couldn't load \"%s\" as white background input.\n",
				white_name);
		return false;
	}

	/* Return failure if there's a problem with the input images. */
	if (!alphagen_check_inputs(&b, &w)) {
		return false;
	}

	/* Prepare output image */
	if (!image_init(&a, b.width, b.height, 4)) {
		printf("Couldn't create output image.\n");
		return false;
	}

	/* Recover true image colours and alpha channel for the image */
	row_data_width = a.width * a.channels;
	for (y = 0; y < a.height; y++) {
		z = 0; /* x-position in input images */
		for (x = 0; x < row_data_width; x += 4) {
			/* Get alpha values first */
			ar = 255 - w.d[y][z]   + b.d[y][z];
			ag = 255 - w.d[y][z+1] + b.d[y][z+1];
			ab = 255 - w.d[y][z+2] + b.d[y][z+2];

			/* Set red, green and blue colour values
			 * use preset bg colour if fully transparent */
			a.d[y][x]   = (ar == 0) ? bg : (b.d[y][z]   * 255) / ar;
			a.d[y][x+1] = (ag == 0) ? bg : (b.d[y][z+1] * 255) / ag;
			a.d[y][x+2] = (ab == 0) ? bg : (b.d[y][z+2] * 255) / ab;

			/* Set alpha value */
			a.d[y][x+3] = (ar + ag + ab) / 3;

			z += 3;
		}
	}

	/* Save PNG with recovered colour and alpha data */
	if (!image_write_png(alpha_name, &a)) {
		printf("Couldn't write output file \"%s\".\n", alpha_name);
		return false;
	}

	/* Free image memory */
	image_free(&b);
	image_free(&w);
	image_free(&a);

	return true;
}


/**
 * Check input images for problems
 *
 * \param  b  black backgrounded input image
 * \param  w  white backgrounded input image
 * \return  true on success, false on error
 */
bool alphagen_check_inputs(struct image *b, struct image *w)
{
	unsigned int data_size;
	unsigned int i;
	bool opaque = true;

	/* Check both input images are the same size */
	if (b->width != w->width || b->height != w->height) {
		printf("Error: Input images have different dimensions.\n");
		return false;
	}

	/* Check input images are both the same bit depth */
	if (b->channels != w->channels || b->channels != 3) {
		/* AlphaGen should load both as 24bpp, so it's not an input
		 * error, but an internal error. */
		printf("Error: Internal error.\n");
		return false;
	}

	/* Check that black background image is darker than white and check
	 * for opaque image (identical input images). */
	data_size = b->height * b->width * b->channels;
	for (i = 0; i < data_size; i++) {
		if (b->d[0][i] > w->d[0][i]) {
			/* Black bg image is brighter than white bg image. */
			printf("Error: Black background input image has pixel "
					"brighter than White background\n"
					"input image.\n");
			return false;
		}
		if (b->d[0][i] < w->d[0][i]) {
			/* White bg image data brighter than black bg image. */
			opaque = false;
		}
	}

	/* If input images were identical, the image is opaque and there
	 * is no alpha channel to recover. */
	if (opaque) {
		printf("Error: Input images are identical.  "
				"No alpha channel to recover.\n");
		return false;
	}

	return true;
}


/**
 * Initialise an image, allocating memory for it.
 *
 * \param  img       pointer to image to initialise
 * \param  width     required image width
 * \param  height    required image height
 * \param  channels  required number of colour channels
 * \return  true on success, false on error
 */
bool image_init(struct image *img, int width, int height, int channels)
{
	int row_data_width;
	int i;

	/* Set frame dimensions */
	img->width = width;
	img->height = height;
	img->channels = channels;

	/* Allocate memory for row pointers */
	img->d = malloc(height * sizeof(uint8_t*));
	if (img->d == NULL)
		return false;

	/* Allocate memory for image data */
	row_data_width = width * channels;
	img->d[0] = malloc(height * row_data_width * sizeof(uint8_t));
	if (img->d[0] == NULL)
		return false;

	for (i = 1; i < height; i++) {
		/* Set pointers to each row */
		img->d[i] = img->d[i - 1] + row_data_width;
	}
	return true;
}


/**
 * Free an image.
 *
 * \param  img  pointer to image to free
 */
void image_free(struct image *img)
{
	free(img->d[0]);
	free(img->d);
}


/**
 * Read RGB PNG with no alpha channel into img.
 *
 * \param  file_name  file to load
 * \param  img        pointer to image to put data in
 * \return  true on success, false on error
 */
bool image_read_png(char *file_name, struct image *img)
{
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	unsigned int row_data_width;
	png_uint_32 width, height;
	int bit_depth, color_type;
	int i;
	FILE *fp;

	if ((fp = fopen(file_name, "rb")) == NULL)
		return false;

	/* Create and initialize the png_struct. */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
			NULL, NULL, NULL);

	if (png_ptr == NULL) {
		fclose(fp);
		return false;
	}

	/* Allocate/initialize the memory for image information. */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL,
				png_infopp_NULL);
		return false;
	}

	/* Set error handling if you are using the setjmp/longjmp method
	 * because I left stuff as NULL in png_create_read_struct(). */
	if (setjmp(png_jmpbuf(png_ptr))) {
		/* Free all of the memory associated with the png_ptr and
		 * info_ptr */
		png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
		fclose(fp);
		/* If we get here, we had a problem reading the file */
		return false;
	}

	/* Set up the input control */
	png_init_io(png_ptr, fp);

	/* If we have already read some of the signature */
	png_set_sig_bytes(png_ptr, sig_read);

	/* Read in the entire image at once */
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_PACKING, png_voidp_NULL);

	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);

	if (color_type != PNG_COLOR_TYPE_RGB && bit_depth != 8)
		return false;

	if (!image_init(img, width, height, 3))
		return false;

	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

	row_data_width = img->width * img->channels;
	for (i = 0; i < img->height; i++) {
		memcpy(img->d[i], row_pointers[i], row_data_width);
	}

	/* clean up after the read, and free any memory allocated */
	png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);

	/* close the file */
	fclose(fp);

	return true;
}


/**
 * Save output PNG file with alpha channel with img data
 *
 * \param  file_name  file to write PNG out to
 * \param  img        image data to generate PNG from
 * \returnreturn  true on success, false on error
 */
bool image_write_png(char *file_name, struct image *img)
{
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	int colour_type;

	switch (img->channels) {
	case 1:
		colour_type = PNG_COLOR_TYPE_GRAY;
		break;
	case 3:
		colour_type = PNG_COLOR_TYPE_RGB;
		break;
	case 4:
		colour_type = PNG_COLOR_TYPE_RGB_ALPHA;
		break;
	default:
		/* Unsupported number of image channels */
		return false;
	}

	/* open the file */
	fp = fopen(file_name, "wb");
	if (fp == NULL)
		return false;

	/* Create and initialize the png_struct */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
			NULL, NULL, NULL);

	if (png_ptr == NULL) {
		fclose(fp);
		return false;
	}

	/* Allocate/initialize the image information data. */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fclose(fp);
		png_destroy_write_struct(&png_ptr, png_infopp_NULL);
		return false;
	}
	/* Set error handling, needed because I gave NULLs to
	 * png_create_write_struct. */
	if (setjmp(png_jmpbuf(png_ptr))) {
		/* If we get here, we had a problem reading the file */
		fclose(fp);
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return false;
	}

	/* set up the output control */
	png_init_io(png_ptr, fp);

	/* Set the image information. */
	png_set_IHDR(png_ptr, info_ptr, img->width, img->height, 8,
			colour_type, PNG_INTERLACE_ADAM7,
			PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	/* TODO: ??? Maybe I should set gamma stuff, but I dunno what */
	/* png_set_gAMA(png_ptr, info_ptr, gamma); */

	/* Write the file header information. */
	png_write_info(png_ptr, info_ptr);

	/* pack pixels into bytes */
	png_set_packing(png_ptr);

	if (img->height > PNG_UINT_32_MAX/png_sizeof(png_bytep)) {
		png_error (png_ptr, "Image is too tall to process in memory");
		return false;
	}

	png_write_image(png_ptr, (png_byte **)img->d);

	/* finish writing the rest of the file */
	png_write_end(png_ptr, info_ptr);

	/* clean up after the write, and free any memory allocated */
	png_destroy_write_struct(&png_ptr, &info_ptr);

	/* close the file */
	fclose(fp);

	return true;
}
