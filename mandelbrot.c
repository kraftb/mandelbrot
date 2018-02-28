#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>

/**
 * Mandelbrot v0.1
 *
 * A simple mandelbrot fractal generator.
 * 
 * (c) 2012-2018 Bernhard Kraft
 *
 * This projectl is free software; you can redistribute it and/or         *
 * modify it under the terms of the GNU General Public License, either    *
 * version 2 of the License, or any later version.                        *
 *                                                                        *
 * This program is distributed in the hope that it will be useful, but    *
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-    *
 * TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General      *
 * Public License for more details. 
 *
 */

#include <SDL.h>

#include <mandel_const.h>
#include <mandel_misc.h>
#include <mandel_alg.h>
#include <mandel_video.h>

int main(int argc, char **argv) {
	int err = 0;
	int x, y;
	Uint32 r = 0, g = 0, b = 0;
	double zx, zy;
	double factE = 255.0/0.1;
	double maxE = 0;
	int minSteps = 1000;
	double e;
	if (err = mandel_parse_args(argc, argv)) {
		exit(err);
	}
	if (err = mandel_video_init()) {
		exit(err);
	}

	mandel_alg_init();
	for (y = 0; y < mandel_height; y++) {
		zy = mandel_start_y + mandel_step_y * y;
		if (err = mandel_lock()) {
			exit(err);
		}
		for (x = 0; x < mandel_width; x++) {
			zx = mandel_start_x + mandel_step_x * x;
			mandel_alg_start(zx, zy);
			while ((mandel_step < MANDEL_STEPS) && !mandel_alg_exceed()) {
				mandel_alg_iterate();
/*
				if (mandel_step == 2) {
					e = mandel_alg_log();
					if (e > maxE) {
						maxE = e;
					}
					e = (e < 0 ? 0 : e);
					e *= factE;
				}
*/
			}
			r = 255-mandel_step;
/*
			if (mandel_step < minSteps) {
				minSteps = mandel_step;
			}
*/
//			g = (e > 255 ? 255 : (Uint32)e);
//			fprintf(stdout, "%d\n", g);
//			e = floor(mandel_alg_arg()*(255.0/(2*M_PI)));
			mandel_set_pixel_RGB(x, y, (Uint8)r%256, (Uint8)0%256, 0%256);
		}
		mandel_unlock();
		mandel_update();
	}
	mandel_alg_status();
	fprintf(stdout, "min Steps = %d\n", minSteps);
	fprintf(stdout, "max Exponent = %g\n", maxE);
	sleep(20);
	

	mandel_video_quit();
}


