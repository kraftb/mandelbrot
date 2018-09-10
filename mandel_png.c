
#define _GNU_SOURCE

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <mandel_video.h>

#include <png.h>
#include <zlib.h>

int mandel_width = 0;
int mandel_height = 0;


void abort_(const char * s, ...)
{
	va_list args;
	va_start(args, s);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
	exit(-1);
}

int x, y;

FILE *surface = NULL;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep *row_pointers;
png_uint_32 row_bytes;


int mandel_lock() {
}
void mandel_unlock() {
}

int mandel_set_pixel_RGB(Uint32 x, Uint32 y, Uint8 r, Uint8 g, Uint8 b) {
	png_uint_32 offset = (y*row_bytes) + (x*3);

	*(row_pointers[y]+(x*3)+0) = r;
	*(row_pointers[y]+(x*3)+1) = g;
	*(row_pointers[y]+(x*3)+2) = b;
}


int mandel_update() {
}

int mandel_video_init() {
	mandel_width = 1920;
	mandel_height = 1200;
/*
	mandel_width = 1024;
	mandel_height = 768;
*/
//	mandel_width = 640;
//	mandel_height = 480;

	color_type = PNG_COLOR_TYPE_RGB;
	bit_depth = 8;


	// Open output file and create PNG structures
	FILE *surface = fopen("output.png", "wb");
	if (!surface) {
		return MANDEL_ERR_PNG;
	}

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr) {
		return MANDEL_ERR_PNG;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		return MANDEL_ERR_PNG;
	}

	// Set PNG input/output and headers
	png_init_io(png_ptr, surface);
	png_set_IHDR(png_ptr, info_ptr, mandel_width, mandel_height, bit_depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png_ptr, info_ptr);

	// Allocate row pointers
	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * mandel_height);
	row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	for (y = 0; y < mandel_height; y++) {
		row_pointers[y] = (png_byte*) malloc(row_bytes);
	}

	if ((row_bytes / mandel_width) != 3) {
		fprintf(stderr, "Unexpected row_bytes!\n");
		exit(-11);
	}

	return MANDEL_OK;
}

void mandel_video_quit() {
	// Write image date
	png_write_image(png_ptr, row_pointers);

	png_write_end(png_ptr, NULL);

	/* cleanup heap allocation */
	for (y = 0; y < mandel_height; y++) {
		free(row_pointers[y]);
	}
	free(row_pointers);
	fflush(surface);
//	fclose(surface);
	fcloseall();
//	surface = NULL;
}

