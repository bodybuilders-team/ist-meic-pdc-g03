#!/bin/bash

make clean && make

for n in {1..10}
do
    bin/life3d-omp 200 128 .5 1000
done

# bin/life3d-omp 1000 64 0.4 0

# bin/life3d-omp 200 128 .5 1000

# bin/life3d-omp 10 512 .4 0

# bin/life3d-omp 3 1024 .4 100