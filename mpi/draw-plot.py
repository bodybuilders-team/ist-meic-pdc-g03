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
    [29.1, 14.7, 7.6, 4.0, 2.7, 1.6, 1.9],
    [49.1, 24.9, 12.8, 6.4, 7.3, 2.2, 1.5],
    [192.5, 96.8, 49.7, 24.9, 20.8, 9.3, 9.2],
    [476.4, 239.1, 123.1, 61.5, 45.1, 18.5, 14.7]
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
