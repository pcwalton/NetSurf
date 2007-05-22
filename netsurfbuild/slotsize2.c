#include <stdio.h>

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
	FILE *f;
	struct header header;
	unsigned int file_size;

	if (argc != 2)
		return 1;

	f = fopen(argv[1], "rb");
	if (!f)
		return 1;

	fread((void*)&header, sizeof(struct header), 1, f);

	fseek(f, 0, SEEK_END);
	file_size = ftell(f);

	fclose(f);

	if ((header.entry >> 24) != 0xEB)
		return 1;

	if (header.rosize + header.rwsize + header.dbsize != file_size)
		if ((header.decompress >> 24) != 0xEB)
			return 1;

	file_size = header.rosize + header.rwsize + 
			header.dbsize + header.zisize + 
			0x8000 /* 32k of scratch space */;

	printf("%d", (file_size / 1024));

	return 0;
}
