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

# Execute each configuration without setting OMP_NUM_THREADS
for config in "${configurations[@]}"; do
    echo "Executing configuration: $config"
    echo "Executing configuration: $config" >> "$output_file"
    
    ./life3d $config 2>&1 | tee -a "$output_file"
    
    echo "Execution completed."
    echo "Execution completed." >> "$output_file"
done
