#ifndef _TTF2F_UTILS_H_
#define _TTF2F_UTILS_H_

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

#ifdef __riscos__
#define DIR_SEP "."
#else
#define DIR_SEP "/"
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef N_ELEMENTS
#define N_ELEMENTS(x) (sizeof((x)) / sizeof((x)[0]))
#endif

typedef enum ttf2f_result {
	TTF2F_RESULT_OK,
	TTF2F_RESULT_NOMEM,
	TTF2F_RESULT_OPEN,
	TTF2F_RESULT_WRITE
} ttf2f_result;

void ttf2f_poll(int active);
char *strndup(const char *s, size_t n);
long convert_units(long raw, long ppem);

#endif
