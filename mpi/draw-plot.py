import matplotlib.pyplot as plt

# Data from the table
configurations = [
    "life3d 1000 64 0.4 0",
    "life3d 200 128 0.5 1000",
    "life3d 10 512 0.4 0",
    "life3d 3 1024 0.4 100"
]

parallel_tasks = [1, 2, 4, 8, 16, 32, 64]

serial_times = [34.6, 59.7, 247.9, 628.0]

mpi_times = [
    [40.1, 28.5, 14.3, 7.3, 3.7, 2.2, 1.3],
    [68.1, 47.7, 24.1, 12.0, 6.1, 3.1, 1.7],
    [266.2, 182.4, 91.7, 45.9, 23.3, 11.6, 5.9],
    [660.1, 452.0, 229.2, 113.7, 57.7, 29.6, 15.5]
]

speedup_mpi = [
    [serial_times[i] / mpi_times[i][j] for j in range(len(parallel_tasks))] for i in range(len(configurations))
]

# ------------------------------
# Plotting serial and MPI execution times
# ------------------------------	

plt.figure(figsize=(10, 6))

for i, config in enumerate(configurations):
    plt.plot(['Serial'] + ['MPI{}'.format(parallel_tasks[j]) for j in range(len(parallel_tasks))], [serial_times[i]] + mpi_times[i], marker='o', label=config)

plt.title('Execution Times vs Parallel Tasks (MPI)')
plt.xlabel('Configuration')
plt.ylabel('Execution Time (s)')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig('imgs/execution_times_mpi.png')  # Save the figure as PNG
plt.show()


# ------------------------------
# Plotting speedup for MPI
# ------------------------------

plt.figure(figsize=(10, 6))

for i in range(len(configurations)):
    plt.plot(parallel_tasks, speedup_mpi[i], marker='o', label=configurations[i])

plt.title('Speedup vs Parallel Tasks (MPI)')
plt.xlabel('Parallel Tasks')
plt.ylabel('Speedup')
plt.xscale('log', base=2)  # Log scale for better visualization of parallel tasks
plt.xticks(parallel_tasks)
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig('imgs/speedup_vs_parallel_tasks_mpi.png')  # Save the figure as PNG
plt.show()
