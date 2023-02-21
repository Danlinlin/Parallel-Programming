#!/bin/bash
for n in 1 2 4 6 8
do
module load openmpi
mpic++ project7.cpp -o project7 -lm
mpiexec -mca btl self,tcp -np $n project7
done
