#ifndef iconv_module_h_
#define iconv_module_h_

#ifndef DEBUG
#define LOG(x)
#else
#define LOG(x) (printf(__FILE__ " %s %i: ", __func__, __LINE__), printf x, fputc('\n', stdout))
#endif

#define UNUSED(x) ((x) = (x))

/* In iconv library */
extern int iconv_eightbit_number_from_name(const char *name);
extern short mibenum_from_name(const char *alias);
extern const char *mibenum_to_name(short mibenum);

/* in menu.c */
size_t iconv_createmenu(size_t flags, char *buf, size_t buflen,
		const char *selected, char *data, size_t *dlen);
size_t iconv_decodemenu(size_t flags, void *menu, int *selections,
		char *buf, size_t buflen);

#endif

