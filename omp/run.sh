#!/bin/bash

make clean && make

for n in {1..10}
do
    ./life3d 200 128 .5 1000
done

# ./life3d-omp 1000 64 0.4 0

# ./life3d-omp 200 128 .5 1000

# ./life3d-omp 10 512 .4 0

# ./life3d-omp 3 1024 .4 100