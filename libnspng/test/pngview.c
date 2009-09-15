#include <stdbool.h>

#include <SDL.h>

#include <libnspng.h>

#define UNUSED(x) ((x)=(x))

typedef struct pngview_ctx {
	nspng_ctx *nspng;
	SDL_Surface *screen;

	uint64_t millis;
	uint64_t count;
} pngview_ctx;

static void *myrealloc(void *ptr, size_t len, void *pw)
{
	UNUSED(pw);

	return realloc(ptr, len);
}

static nspng_error row_handler(const uint8_t *row, uint32_t rowbytes,
		uint32_t rownum, int pass, void *pw)
{
	pngview_ctx *ctx = pw;
	void *vscanline = ((uint8_t *) ctx->screen->pixels + 
			rownum * ctx->screen->pitch);
	uint32_t *scanline = (uint32_t *) vscanline;
	uint32_t col;

	UNUSED(pass);

	for (col = 0; col != rowbytes >> 2; col++) {
		const uint8_t *pix = row + col * 4;

		*(scanline + col) = (pix[0] << ctx->screen->format->Rshift) |
				    (pix[1] << ctx->screen->format->Gshift) |
				    (pix[2] << ctx->screen->format->Bshift) |
				    /* SDL alpha is inverse */
				    ((0xff - pix[3]) << 
						ctx->screen->format->Ashift);
	}

	return NSPNG_OK;
}

static nspng_error redraw_window(pngview_ctx *ctx)
{
	int success;
	nspng_rect clip;
	nspng_error error;
	uint32_t start, end;

	clip.x0 = 0;
	clip.y0 = 0;
	clip.x1 = ctx->screen->w;
	clip.y1 = ctx->screen->h;

	if (SDL_MUSTLOCK(ctx->screen)) {
		success = SDL_LockSurface(ctx->screen);
		if (success == -1)
			return NSPNG_INVALID;
	}

	SDL_FillRect(ctx->screen, NULL, SDL_MapRGB(ctx->screen->format, 0, 0, 0));

	start = SDL_GetTicks();

	error = nspng_render(ctx->nspng, &clip, row_handler, ctx);
	if (error != NSPNG_OK) {
		printf("FAIL: nspng_render: %d\n", error);
		return error;
	}

	end = SDL_GetTicks();

	if (SDL_MUSTLOCK(ctx->screen)) {
		SDL_UnlockSurface(ctx->screen);
	}

	SDL_UpdateRect(ctx->screen, 0, 0, ctx->screen->w, ctx->screen->h);

	ctx->millis += end - start;
	ctx->count++;

	return NSPNG_OK;
}

int main(int argc, char **argv)
{
	int success;
	FILE *fp;
	uint8_t buf[4096];
	size_t read;
	pngview_ctx ctx;
	uint32_t width, height;
	nspng_error error;
	bool done = false;
	SDL_Event event;

	UNUSED(argc);

	memset(&ctx, 0, sizeof(pngview_ctx));

	success = SDL_Init(SDL_INIT_VIDEO);
	if (success == -1)
		return EXIT_FAILURE;

	fp = fopen(argv[1], "rb");
	if (fp == NULL) {
		printf("FAIL: couldn't open %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	error = nspng_ctx_create(myrealloc, NULL, &ctx.nspng);
	if (error != NSPNG_OK) {
		printf("FAIL: nspng_ctx_create: %d\n", error);
		return EXIT_FAILURE;
	}

	while ((read = fread(buf, 1, sizeof(buf), fp)) > 0) {
		error = nspng_process_data(ctx.nspng, buf, read);
		if (error != NSPNG_OK && error != NSPNG_NEEDDATA) {
			printf("FAIL: nspng_process_data: %d\n", error);
			return EXIT_FAILURE;
		}
	}

	error = nspng_get_dimensions(ctx.nspng, &width, &height);
	if (error != NSPNG_OK) {
		printf("FAIL: nspng_get_dimensions: %d\n", error);
		return EXIT_FAILURE;
	}

	ctx.screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);
	if (ctx.screen == NULL)
		return EXIT_FAILURE;

	while (done == false) {
		while (SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_QUIT:
				done = true;
				break;
			case SDL_VIDEOEXPOSE:
				redraw_window(&ctx);
				break;
			}
		}

		redraw_window(&ctx);

		SDL_Delay(500);
	}

	nspng_ctx_destroy(ctx.nspng);

	fclose(fp);

	SDL_Quit();

	printf("Mean redraw time: %f ms\n", (double) ctx.millis / ctx.count);

	return EXIT_SUCCESS;
}

