#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <CL/cl.h>

/**
 * Mandelbrot v0.2
 *
 * A simple mandelbrot fractal generator using OpenCL for calculations
 * 
 * (c) 2012-2018 Bernhard Kraft
 *
 * This project is free software; you can redistribute it and/or         *
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

typedef float number_type;

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

int main(int argc, char **argv) {

    const char *KernelSource =
        #include "mandel_kernel.cl"
    ;

	int err = 0;
    cl_int cl_error = 0;
	int x, y;
	unsigned int r = 0, g = 0, b = 0;
	double zx, zy;
	double factE = 255.0/0.1;
	double maxE = 0;
	int minSteps = 1000;
    size_t pixels_count = 0;
    int pixels_mem = 0;
    unsigned int *pixels_red = NULL;
    unsigned int *pixels_green = NULL;
    unsigned int *pixels_blue = NULL;
	double e;
    mandel_params params;
    size_t work_group_size = 0;

	if (err = mandel_parse_args(argc, argv)) {
		exit(err);
	}
	if (err = mandel_video_init()) {
		exit(err);
	}

/*
    fprintf(stderr, "kernel source:\n%s\n\n", KernelSource);
    exit(0);
*/

	mandel_alg_init();

    // Initialize mandel params
    params.start_x = (number_type)mandel_start_x;
    params.start_y = (number_type)mandel_start_y;
    params.step_x = (number_type)mandel_step_x;
    params.step_y = (number_type)mandel_step_y;
    params.width = mandel_width;
    params.height = mandel_height;
    params.steps = (int)MANDEL_STEPS;
    params.exceed = MANDEL_EXCEED;

/*
    OpenCL initialization and code execution ------ begin
*/

    // Get platform and device information
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;   
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret;

    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
 
    clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &work_group_size, NULL);
    #ifdef DEBUG
        fprintf(stderr, "Using %d parallel work units.\n", work_group_size);
    #endif

    if (mandel_start_x == -10 && mandel_start_y == -10 && mandel_end_x == 10 && mandel_end_y == 10) {
        cl_uint compute_units;
        clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(size_t), &compute_units, NULL);
        fprintf(stdout, "%d\n", compute_units);
        exit(0);
    }

    // Create an OpenCL context
    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
 
    // Create a command queue
    cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, device_id, NULL, &ret);

    // Initialize pixel buffer(s)
    pixels_count = mandel_height * mandel_width;
    pixels_mem = pixels_count * sizeof(unsigned int);

    pixels_red = (unsigned int*)malloc(pixels_mem);
    memset((void*)pixels_red, 0, pixels_mem);

//    pixels_green = (unsigned int*)malloc(pixels_mem);
//    memset((void*)pixels_green, 0, pixels_mem);

//    pixels_blue = (unsigned int*)malloc(pixels_mem);
//    memset((void*)pixels_blue, 0, pixels_mem);

    // Create memory buffers on the device for each vector
    cl_mem params_mem = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(mandel_params), NULL, &ret);
    cl_mem red_mem = clCreateBuffer(context, CL_MEM_WRITE_ONLY, pixels_mem, NULL, &ret);
//    cl_mem green_mem = clCreateBuffer(context, CL_MEM_WRITE_ONLY, pixels_mem, NULL, &ret);
//    cl_mem blue_mem = clCreateBuffer(context, CL_MEM_WRITE_ONLY, pixels_mem, NULL, &ret);
 
    // Copy the mandel parameters to the allocated memory buffer
    ret = clEnqueueWriteBuffer(command_queue, params_mem, CL_TRUE, 0, sizeof(mandel_params), &params, 0, NULL, NULL);

    // Copy the pixel buffer to their respective memory buffers
    ret = clEnqueueWriteBuffer(command_queue, red_mem, CL_TRUE, 0, pixels_mem, pixels_red, 0, NULL, NULL);
//    ret = clEnqueueWriteBuffer(command_queue, green_mem, CL_TRUE, 0, pixels_mem, pixels_green, 0, NULL, NULL);
//    ret = clEnqueueWriteBuffer(command_queue, blue_mem, CL_TRUE, 0, pixels_mem, pixels_blue, 0, NULL, NULL);
 
    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&KernelSource, NULL, &cl_error);
    if ( (program == NULL) || (cl_error != CL_SUCCESS) ) {
        fprintf(stderr, "Error creating OpenCL program. (%d)\n", cl_error);
        exit(1);
    }
 
    // Build the program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    if (ret != CL_SUCCESS) {
        char error_log[10000];
        size_t error_length;
        fprintf(stderr, "Error compiling OpenCL program. (%d)\n", ret);
        ret = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 10000, error_log, &error_length);
        fprintf(stderr, "%s\n", error_log);
        exit(1);
    }
 
    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "mandel_kernel", &ret);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Error creating OpenCL kernel. (%d)\n", ret);
        exit(1);
    }

 
    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(mandel_params), (void *)&params);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Error setting OpenCL kernel argument. (1: %d)\n", ret);
        exit(1);
    }
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&red_mem);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Error setting OpenCL kernel argument. (2: %d)\n", ret);
        exit(1);
    }
//    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&red_mem);
//    ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&red_mem);
 

    // Execute the OpenCL kernel on the list
    size_t local_item_size = work_group_size; // Divide work items into groups of "work_group_size"
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &pixels_count, &local_item_size, 0, NULL, NULL);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Error executing OpenCL kernel. (%d)\n", ret);
        exit(1);
    }
 
    // Read the memory buffer C on the device to the local variable C
    ret = clEnqueueReadBuffer(command_queue, red_mem, CL_TRUE, 0, pixels_mem, pixels_red, 0, NULL, NULL);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Error reading buffer. (%d)\n", ret);
        exit(1);
    }
//    ret = clEnqueueReadBuffer(command_queue, green_mem, CL_TRUE, 0, pixels_mem, pixels_green, 0, NULL, NULL);
//    ret = clEnqueueReadBuffer(command_queue, blue_mem, CL_TRUE, 0, pixels_mem, pixels_blue, 0, NULL, NULL);

/*
    OpenCL initialization and code execution ------ end
*/

    if (err = mandel_lock()) {
        exit(err);
    }

	for (y = 0; y < mandel_height; y++) {
		for (x = 0; x < mandel_width; x++) {
            r = *(pixels_red + (y * mandel_width) + x);
			mandel_set_pixel_RGB(x, y, (Uint8)r%256, (Uint8)0%256, 0%256);
		}
	}

    mandel_unlock();
    mandel_update();

    #ifdef DEBUG
        fprintf(stdout, "min Steps = %d\n", minSteps);
        fprintf(stdout, "max Exponent = %g\n", maxE);
    #endif
	
	mandel_video_quit();
}

