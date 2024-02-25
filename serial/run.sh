#!/bin/bash

make clean && make

for n in {1..10}
do
    bin/life3d 1000 64 0.4 0
done

# bin/life3d 1000 64 0.4 0

# bin/life3d 200 128 .5 1000

# bin/life3d 10 512 .4 0

# bin/life3d 3 1024 .4 100