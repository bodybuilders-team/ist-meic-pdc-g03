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

# Output file
output_file="output.txt"

# Define parallel tasks
# TODO: change to control number of threads in each processor using OMP_NUM_THREADS
parallel_tasks=(1 2 4 8 16 32 64)

# Define hosts in lab
hosts="lab1p1,lab1p2,lab1p3,lab1p4,lab1p5,lab1p6,lab1p7,lab1p8,lab1p9,lab1p10"

# Loop through each configuration
for config in "${configurations[@]}"; do
    for processes in "${parallel_tasks[@]}"; do
        echo "Executing with $processes MPI processes and configuration: $config"
        mpirun -host $hosts -np $processes ./life3d-mpi $config >> "$output_file" 2>&1
        echo "Execution completed."
    done
done
