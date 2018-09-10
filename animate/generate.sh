#!/bin/bash

INDEX=$1
FILE=$2
X_MIN=$3
Y_MIN=$4
X_MAX=$5
Y_MAX=$6

echo "output/$INDEX"

mkdir "output/$INDEX"
cd "output/$INDEX"
../../mandelbrot $X_MIN $Y_MIN $X_MAX $Y_MAX
mv output.png ../$FILE
cd ..
rmdir $INDEX

