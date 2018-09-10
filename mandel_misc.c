#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mandel_const.h>
#include <mandel_alg.h>

int mandel_parse_args(int argc, char **argv) {
	if (argc < 5) {
		fprintf(stderr, "Usage: ./mandelbrot Xmin Ymin Xmax Ymax\n");
		fprintf(stderr, "       Xmin < Xmax\n");
		fprintf(stderr, "       Ymin < Ymax\n");
		fprintf(stderr, "       When Xmin, Ymin, Xmax and Ymax are \"0\" default values get used!\n");
		return MANDEL_ERR_ARGCOUNT;
		return 1;
	}
	mandel_start_x = atof(argv[1]);
	mandel_start_y = atof(argv[2]);
	mandel_end_x = atof(argv[3]);
	mandel_end_y = atof(argv[4]);
	if (!(strcmp(argv[1], "0") || strcmp(argv[2], "0") || strcmp(argv[3], "0") || strcmp(argv[4], "0"))) {
		mandel_start_x = -2;
		mandel_end_x = 2;
		mandel_start_y = -2;
		mandel_end_y = 2;
	}
	if (mandel_start_x >= mandel_end_x) {
		fprintf(stderr, "Error: Xmin must be smaller than Xmax!\n");
		return MANDEL_ERR_XMINMAX;
	}
	if (mandel_start_y >= mandel_end_y) {
		fprintf(stderr, "Error: Ymin must be smaller than Ymax!\n");
		return MANDEL_ERR_YMINMAX;
	}
	mandel_diff_x = mandel_end_x - mandel_start_x;
	mandel_diff_y = mandel_end_y - mandel_start_y;
	return 0;
}

