#include <stdio.h>
#include <stdlib.h>

#ifdef __riscos__
#include <swis.h>
#endif

#include <libnspng.h>

#include "../src/internal.h"

#define UNUSED(x) ((x)=(x))

static void *myrealloc(void *ptr, size_t len, void *pw)
{
	UNUSED(pw);

	return realloc(ptr, len);
}

static nspng_error row_handler(const uint8_t *row, uint32_t rowbytes,
		uint32_t rownum, int pass, void *pw)
{
//	uint32_t col;

	UNUSED(row);
	UNUSED(rowbytes);
	UNUSED(rownum);
	UNUSED(pass);
	UNUSED(pw);
#if 0
	for (col = 0; col != rowbytes >> 2; col++) {
		const void *ppix = row + col * 4;
		uint32_t pix = *((const uint32_t *) ppix);

		printf("%u %u %u ", pix >> 24, (pix >> 16) & 0xff, 
				(pix >> 8) & 0xff);
	}

	printf("\n");
#endif
	return NSPNG_OK;
}

int main(int argc, char **argv)
{
	FILE *fp;
	uint8_t buf[4096];
	size_t read;
	nspng_ctx *ctx;
	nspng_rect clip;
	uint32_t width;
	uint32_t height;
	nspng_error error;

	uint64_t full = 0, comp = 0;

	UNUSED(argc);

	for (int i = 1; i < argc; i++) {

		fp = fopen(argv[i], "rb");
		if (fp == NULL) {
			printf("FAIL: couldn't open %s\n", argv[i]);
			return 1;
		}

		error = nspng_ctx_create(myrealloc, NULL, &ctx);
		if (error != NSPNG_OK) {
			printf("FAIL: nspng_ctx_create: %d\n", error);
			return 1;
		}

#ifdef __riscos__
		int startTime, endTime;
		_swix(OS_ReadMonotonicTime, _OUT(0), &startTime);
#endif

		while ((read = fread(buf, 1, sizeof(buf), fp)) > 0) {
			error = nspng_process_data(ctx, buf, read);
			if (error != NSPNG_OK && error != NSPNG_NEEDDATA) {
				printf("FAIL: nspng_process_data: %d\n", error);
				return 1;
			}
		}

#ifdef __riscos__
		_swix(OS_ReadMonotonicTime, _OUT(0), &endTime);
		printf("Process: %d cs\n", endTime - startTime);
#endif

		error = nspng_get_dimensions(ctx, &width, &height);
		if (error != NSPNG_OK) {
			printf("FAIL: nspng_get_dimensions: %d\n", error);
			return 1;
		}

//		printf("P3\n");
//		printf("%u %u 256\n", width, height);

		clip.x0 = 0;
		clip.y0 = 0;
		clip.x1 = width;
		clip.y1 = height;

#ifdef __riscos__
		_swix(OS_ReadMonotonicTime, _OUT(0), &startTime);
#endif

		error = nspng_render(ctx, &clip, row_handler, NULL);
		if (error != NSPNG_OK) {
			printf("FAIL: nspng_render: %d\n", error);
			return 1;
		}

#ifdef __riscos__
		_swix(OS_ReadMonotonicTime, _OUT(0), &endTime);
		printf("Render : %d cs\n", endTime - startTime);
#endif

		fseek(fp, 0, SEEK_END);
		long fsize = ftell(fp);

		full += ctx->image.height * ctx->image.bytes_per_scanline;
		comp += ctx->image.data_len;

		printf("%s: %lu (%u) -> %u = %.2f%%\n", argv[i], fsize, 
			ctx->image.height * ctx->image.bytes_per_scanline,
			ctx->image.data_len,
			(float) ctx->image.data_len * 100 / 
			(float) (ctx->image.height * ctx->image.bytes_per_scanline));

		nspng_ctx_destroy(ctx);

		fclose(fp);
	}

	printf("Mean: %lu -> %lu = %.2f%%\n", full, comp, 
		(float) comp * 100 / (float) full);

	//printf("PASS\n");

	return 0;
}
