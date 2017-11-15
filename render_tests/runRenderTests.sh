#!/bin/bash
cd build/SpheresTestBench/
# just to be sure ...
make -j4
if [ $? -ne 0 ]; then
    echo "Compile failed"
    exit 1
fi
# make sure the old ones are deleted
rm benchmark_out.bmp
./SpheresTestBench --automate
compare ../../render_tests/CubeBenchmark.bmp  benchmark_out.bmp  -compose src out.bmp
if [ $? -ne 0 ]; then
    echo "CubeBenchmark is not equal to reference"
    exit 1
fi
echo "All Render tests successful !!"

