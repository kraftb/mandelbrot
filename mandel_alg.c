#include <stdio.h>
#include <math.h>
#include <complex.h>

#include <mandel_alg.h>
#include <mandel_video.h>

double mandel_start_x = 0;
double mandel_end_x = 0;
double mandel_diff_x = 0;
double mandel_step_x = 0;

double mandel_start_y = 0;
double mandel_end_y = 0;
double mandel_diff_y = 0;
double mandel_step_y = 0;

double complex mandel_c = 0;
double complex mandel_z = 0;
int mandel_step = 0;

void mandel_alg_init() {
	mandel_step_x = mandel_diff_x / (mandel_width*1.0);
	mandel_step_y = mandel_diff_y / (mandel_height*1.0);
}

void mandel_alg_start(double x, double y) {
	mandel_z = 0;
	mandel_c = x + y*I;
	mandel_step = 0;
}

void mandel_alg_iterate() {
	mandel_z = mandel_z*mandel_z + mandel_c;
	mandel_step++;
}

double mandel_alg_log() {
	return log(cabs(mandel_z));
}
double mandel_alg_arg() {
	return carg(mandel_z)+M_PI;
}

int mandel_alg_exceed() {
	double abs = cabs(mandel_z);
	if (abs > MANDEL_EXCEED) {
		return 1;
	} else {
		return 0;
	}
}

void mandel_alg_status() {
	fprintf(stdout, "z%d = %g + %gj = %g * e^(j*%g)\n", mandel_step, creal(mandel_z), cimag(mandel_z), cabs(mandel_z), carg(mandel_z));
}


