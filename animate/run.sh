#!/bin/bash

CPU_COUNT=4

rm -rf output && mkdir output

./anim.php | xargs -L 1 -P $CPU_COUNT ./generate.sh

