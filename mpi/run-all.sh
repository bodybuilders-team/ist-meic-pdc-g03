#!/bin/bash

# Compile the program
make clean && make

# Define configurations
configurations=(
    "1000 64 0.4 0"
    "200 128 0.5 1000"
    "10 512 0.4 0"
    "3 1024 0.4 100"
)

# Define thread counts
thread_counts=(1 2 4 8 16 32 64)

# Loop through each configuration
for config in "${configurations[@]}"; do
    for threads in "${thread_counts[@]}"; do
        export OMP_NUM_THREADS=$threads
        echo "Executing with OMP_NUM_THREADS=$threads and configuration: $config"
        ./life3d $config
        echo "Execution completed."
    done
done
