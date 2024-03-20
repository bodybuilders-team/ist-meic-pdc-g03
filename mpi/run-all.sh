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

# Define parallel tasks
nr_parallel_tasks=(1 2 4 8 16 32 64)

# Define hosts in lab
hosts="lab1p1,lab1p2,lab1p3,lab1p4,lab1p5,lab1p6,lab1p7,lab1p8,lab1p9,lab1p10"

# Loop through each configuration
for config in "${configurations[@]}"; do
    for parallel_tasks in "${nr_parallel_tasks[@]}"; do
        
        echo "Executing with $parallel_tasks MPI processes and configuration: $config" 
        echo "Executing with $parallel_tasks MPI processes and configuration: $config" >> "$output_file"

        # srun –cpus-per-task=<n> -n <number of processes>  ./life3d-mpi <config>
        srun -n $parallel_tasks ./life3d-mpi $config 2>&1 | tee -a "$output_file"
        echo "Execution completed."
        echo "Execution completed." >> "$output_file"
    done
done
