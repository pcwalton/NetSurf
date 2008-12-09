#include <stdio.h>
#include <string.h>

#ifdef __riscos
  #include <unixlib/local.h>
  #include <oslib/osfile.h>
#endif

struct header {
	unsigned int decompress;
	unsigned int selfreloc;
	unsigned int zeroinit;
	unsigned int entry;
	unsigned int exit;
	unsigned int rosize;
	unsigned int rwsize;
	unsigned int dbsize;
	unsigned int zisize;
	unsigned int dbtype;
	unsigned int base;
	unsigned int wkspace;
};

int main(int argc, char **argv)
{
	FILE *f, *g;
	struct header header;
	unsigned int file_size;
	char buf[256];

	if (argc != 4) {
		fprintf(stderr, "Usage: %s <runimage> <input> <output>\n", 
				argv[0]);
		return 1;
	}

	f = fopen(argv[1], "rb");
	if (f == NULL) {
		fprintf(stderr, "Failed opening %s\n", argv[1]);
		return 1;
	}

	fread((void*)&header, sizeof(struct header), 1, f);

	fseek(f, 0, SEEK_END);
	file_size = ftell(f);

	fclose(f);

	if ((header.entry >> 24) != 0xEB) {
		fprintf(stderr, "%s not binary\n", argv[1]);
		return 1;
	}

	if (header.rosize + header.rwsize + header.dbsize != file_size) {
		if ((header.decompress >> 24) != 0xEB) {
			fprintf(stderr, "Mismatched field sizes\n");
			return 1;
		}
	}

	file_size = header.rosize + header.rwsize + 
			header.dbsize + header.zisize + 
			0x8000 /* 32k of scratch space */;

	f = fopen(argv[2], "r");
	if (f == NULL) {
		fprintf(stderr, "Failed opening %s\n", argv[2]);
		return 1;
	}

	g = fopen(argv[3], "w");
	if (g == NULL) {
		fclose(f);
		fprintf(stderr, "Failed opening %s\n", argv[3]);
		return 1;
	}

	while (fgets(buf, sizeof(buf), f) != NULL) {
		if (strncmp(buf, "WIMPSLOT\n", 9) == 0) {
			fprintf(g, "WimpSlot -min %dk -max %dk\n",
					(file_size / 1024), (file_size / 1024));
		} else {
			fputs(buf, g);
		}
	}

	fclose(g);
	fclose(f);

#ifdef __riscos
	if (__riscosify(argv[3], 0, __RISCOSIFY_STRICT_UNIX_SPECS | 
			__RISCOSIFY_NO_SUFFIX | __RISCOSIFY_FILETYPE_NOT_SET, 
			buf, sizeof(buf), NULL) == NULL) {
		fprintf(stderr, "Riscosify failed\n");
		return 1;
	}

	osfile_set_type(buf, osfile_TYPE_OBEY);
#endif

	return 0;
}

