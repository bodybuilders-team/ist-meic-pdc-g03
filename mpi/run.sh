#!/bin/bash

make clean && make

for n in {1..10}
do
    mpirun -host lab1p8,lab1p10 -np 4 ./life3d 1000 64 0.4 0
done

# ./life3d 1000 64 0.4 0

# ./life3d 200 128 .5 1000

# ./life3d 10 512 .4 0

# ./life3d 3 1024 .4 100