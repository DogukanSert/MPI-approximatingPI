#!/bin/bash
make

./main-serial 1000
./main-serial 10000
./main-serial 100000
./main-serial 1000000

mpirun -np 2 ./main-parallel 1000
mpirun -np 2 ./main-parallel 10000
mpirun -np 2 ./main-parallel 100000
mpirun -np 2 ./main-parallel 1000000
