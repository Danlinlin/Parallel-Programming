#!/bin/bash
for a in 8 32 64 128 256 512
do
        for b in 1 512 1024 2048 4096 8192
        do
                g++ -DNMB=$b -DLOCAL_SIZE=$a -o first first.cpp /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
                ./first 2>> first.csv
        done
done