#!/bin/bash

cd output

mencoder mf://*.png -fps 60 -ovc lavc -lavcopts vcodec=mjpeg -o output.avi

