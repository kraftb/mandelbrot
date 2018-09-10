
R"(

typedef float number_type;

// typedef double2 complex;
typedef float2 complex;

typedef struct {
    number_type start_x;
    number_type start_y;
    number_type step_x;
    number_type step_y;
    int width;
    int height;
    int steps;
    int exceed;
} mandel_params;

#define I ((complex)(0.0, 1.0))

inline complex complex_multiply(complex a, complex b) {
    return (complex)( a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}

inline complex complex_add(complex a, complex b) {
    return (complex)( a.x + b.x, a.y + b.y );
}

// __kernel void mandel_kernel(const mandel_params params, __global unsigned int *red, __global unsigned int *green, __global unsigned int *blue)
__kernel void mandel_kernel(const mandel_params params, __global unsigned int *red)
{
    int i = get_global_id(0);

    complex mandel_z = (complex)(0.0, 0.0);
    complex mandel_c = (complex)(0.0, 0.0);

    int mandel_step = 0;
    float abs = 0;

	number_type zx, zy;

    int y = (int)(i / params.width);
    int x = (int)(i % params.width);

    zy = params.start_y + params.step_y * y;
    zx = params.start_x + params.step_x * x;

    mandel_c = (complex)(zx, zy);

    while ((mandel_step < params.steps) && (abs <= params.exceed)) {
        mandel_z = complex_add(complex_multiply(mandel_z, mandel_z), mandel_c);
        mandel_step++;
        abs = length(mandel_z);
    }

    red[i] = 255 - mandel_step;
}

)"

