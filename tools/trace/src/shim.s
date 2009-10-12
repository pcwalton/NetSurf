#if !defined(__aof__)
		.section	".text"

		.global	image__ro__base
		.global	image__ro__limit

image__ro__base:
		.word	Image$$RO$$Base
image__ro__limit:
		.word	Image$$RO$$Limit
#else
		AREA	|ARM$$Code|, CODE, READONLY

		IMPORT	|Image$$RO$$Base|
		IMPORT	|Image$$RO$$Limit|

		EXPORT	image__ro__base
		EXPORT	image__ro__limit

image__ro__base
		DCD	|Image$$RO$$Base|
image__ro__limit
		DCD	|Image$$RO$$Limit|

		END
#endif
