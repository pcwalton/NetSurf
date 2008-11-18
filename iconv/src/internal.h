#ifndef _ICONV_INTERNAL_H_
#define _ICONV_INTERNAL_H_

#ifndef unicode_encoding_h
#include <unicode/encoding.h>
#endif

#ifndef DEBUG
#define LOG(x)
#else
#include <stdio.h>
#define LOG(x) (printf(__FILE__ " %s %i: ", __func__, __LINE__), printf x, fputc('\n', stdout))
#endif

#define UNUSED(x) ((x) = (x))

struct encoding_context {
	Encoding *in;
	unsigned int inflags;
	Encoding *out;
	unsigned int outflags;
	unsigned short *intab, *outtab;
	char **outbuf;
	size_t *outbytesleft;
	char transliterate;
	enum {
		WRITE_SUCCESS, 
		WRITE_FAILED, 
		WRITE_NOMEM, 
		WRITE_NONE
	} write_state;
	struct encoding_context *prev, *next;
};

/* in eightbit.c */
int iconv_eightbit_number_from_name(const char *name);
unsigned iconv_eightbit_read(struct encoding_context *e,
		int (*callback)(void *handle, UCS4 c), const char *s,
		unsigned int n, void *handle);
int iconv_eightbit_write(struct encoding_context *e, UCS4 c,
		char **buf, int *bufsize);
unsigned short *iconv_eightbit_new(int enc_num);
void iconv_eightbit_delete(struct encoding_context *e);

/* in alias.c */
int iconv_encoding_number_from_name(const char *name);
const char *iconv_encoding_name_from_number(int number);

struct canon {
	struct canon *next;
	short mib_enum;
	unsigned short name_len;
	char name[1];
};

/* in aliases.c */
int create_alias_data(const char *filename);
void free_alias_data(void);
struct canon *alias_canonicalise(const char *alias);
short mibenum_from_name(const char *alias);
const char *mibenum_to_name(short mibenum);

/* in utils.c */
int strcasecmp(const char *s1, const char *s2);
int strncasecmp(const char *s1, const char *s2, size_t len);

#endif
