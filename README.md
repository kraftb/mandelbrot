# mandelbrot

A simple mandelbrot fractal generator

Requires libSDL<2. Dev version.

A simple "make" on the commandline should generated the exectuable.

Zoom parameters can be given on the commandline.

Example call: ./mandelbrot -0.656 0.354 -0.654 0.356

Default view: ./mandelbrot 0 0 0 0

By default the PNG output variant will get compiled.
PNG width and height can currently only get adjusted
directly in mandel_png.c


A call to "make opencl" will generate the opencl enabled version.
The OpenCL enabled version will automatically use the maximum
number of work units per workgroup. If there are multiple compute
units more than one process could get started per time.

See script "run_opencl.sh" in the animate directory.

For the scripts in the "animate" directory to work the
executables have to get copied to the animate directory.

When generating an animation the aspects (number of frames,
zoom target, etc.) can get adjusted in anim.php

