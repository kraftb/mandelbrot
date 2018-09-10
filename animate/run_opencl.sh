#!/bin/bash

CPU_COUNT=`./mandelbrot_opencl -10 -10 10 10`

rm -rf output && mkdir output

./anim.php | xargs -L 1 -P $CPU_COUNT ./generate_opencl.sh



