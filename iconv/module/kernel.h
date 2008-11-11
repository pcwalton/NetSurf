/* Our own, minimal, kernel.h */

#ifndef iconv_myswis_h_
#define iconv_myswis_h_

#include <stdint.h>

typedef struct _kernel_oserror {
	int errnum;
	char errmess[252];
} _kernel_oserror;

/* Why intptr_t? Because it's got a chance of working on 64bit machines */
typedef struct _kernel_swi_regs {
	intptr_t r[10];
} _kernel_swi_regs;

#endif

