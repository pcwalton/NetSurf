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

void ttf2f_poll(int active);
char *strndup(const char *s, size_t n);
long convert_units(long raw, long ppem);

#endif
