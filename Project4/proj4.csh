#!/bin/csh
#number of threads:
foreach t ( 1024 4096 16384 262144 1048576 4194304 8388608)
	g++ -DARRAYSIZE=$t project4.cpp -o proj4 -lm -fopenmp
	./proj4
end