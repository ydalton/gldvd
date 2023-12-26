#include <stdlib.h>
#include <string.h>

#include <png.h>

#include "image.h"

#define PNG_NUM_BYTES 8

char *png_to_fb(const char *filename, unsigned int *width, unsigned int *height)
{
	png_structp png;
	png_bytepp row_pointers;
	png_infop info;
	FILE *fp;
	char *buf = NULL;
	int bit_depth;
	unsigned int row_bytes;

	fp = fopen(filename, "rb");
	if(!fp)
		return NULL;

	/* check if file is png */
	/*
	buf = calloc(1, PNG_NUM_BYTES);
	fread(buf, sizeof(char), PNG_NUM_BYTES, fp);
	if(png_sig_cmp(buf, 0, 4)) {
		free(buf);
		buf = NULL;
		goto out;
	}
	*/

	free(buf);

	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png)
		goto out;

	info = png_create_info_struct(png);
	if(!info)
		goto out;

	if(setjmp(png_jmpbuf(png)))
		goto setjmp_fail;

	png_init_io(png, fp);

	png_read_png(png, info, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING 
		     | PNG_TRANSFORM_EXPAND, NULL);

	png_get_IHDR(png, info, width, height, &bit_depth, NULL, NULL, NULL, 
			NULL);

	row_bytes = png_get_rowbytes(png, info);
	buf = malloc(row_bytes * (*height));

	row_pointers = png_get_rows(png, info);

	for(unsigned int i = 0; i < *height; i++) {
		memcpy(buf + (row_bytes * (*height - 1 - i)), row_pointers[i], row_bytes);
	}

setjmp_fail:
	png_destroy_read_struct(&png, &info, NULL);
out:
	fclose(fp);
	return buf;
}
