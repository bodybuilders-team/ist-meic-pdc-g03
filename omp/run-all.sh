#!/bin/bash

# Output file
output_file="output.txt"

# Compile the program
(make clean && make) 2>&1 | tee -a "$output_file"

# Define configurations
configurations=(
    "1000 64 0.4 0"
    "200 128 0.5 1000"
    "10 512 0.4 0"
    "3 1024 0.4 100"
)

# Define thread counts
thread_counts=(1 2 4 8)

# Loop through each configuration
for config in "${configurations[@]}"; do
    for threads in "${thread_counts[@]}"; do
        export OMP_NUM_THREADS=$threads
        echo "Executing with OMP_NUM_THREADS=$threads and configuration: $config"
        echo "Executing with OMP_NUM_THREADS=$threads and configuration: $config" >> "$output_file"
        
        ./life3d-omp $config 2>&1 | tee -a "$output_file"
        
        echo "Execution completed."
        echo "Execution completed." >> "$output_file"
    done
done