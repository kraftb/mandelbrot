#ifndef MANDEL_ALG_H
#define MANDEL_ALG_H

extern double mandel_start_x;
extern double mandel_end_x;
extern double mandel_diff_x;
extern double mandel_step_x;

extern double mandel_start_y;
extern double mandel_end_y;
extern double mandel_diff_y;
extern double mandel_step_y;

extern int mandel_step;

void mandel_alg_init();
void mandel_alg_start(double x, double y);
void mandel_alg_iterate();
void mandel_alg_status();
int mandel_alg_exceed();
void mandel_alg_status();
double mandel_alg_log();

#endif
