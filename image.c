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
	int color_type;
	unsigned int row_bytes;

	fp = fopen(filename, "rb");
	if(!fp)
		return NULL;

	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png)
		goto out;

	info = png_create_info_struct(png);
	if(!info)
		goto out;

	color_type = png_get_color_type(png, info);
	if(color_type == PNG_COLOR_TYPE_RGB
	   || color_type == PNG_COLOR_TYPE_GRAY
	   || color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

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

	png_destroy_read_struct(&png, &info, NULL);
out:
	fclose(fp);
	return buf;
}

int file_is_png(const char *name)
{
	/* TODO: actually do stuff */
	return 1;
}
