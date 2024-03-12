import matplotlib.pyplot as plt

# Data from the table
configurations = [
    "life3d 1000 64 0.4 0",
    "life3d 200 128 0.5 1000",
    "life3d 10 512 0.4 0",
    "life3d 3 1024 0.4 100"
]

threads = [1, 2, 4]

serial_times = [34.6, 59.7, 247.9, 628.0]

openmp_times = [
    [36.2, 18.1, 9.2],
    [62.0, 31.1, 15.6],
    [250.4, 125.2, 62.7],
    [635.5, 317.8, 159.0]
]

speedup = [
    [0.96, 1.91, 3.76],
    [0.96, 1.92, 3.83],
    [0.99, 1.98, 3.95],
    [0.99, 1.98, 3.95]
]

# ------------------------------
# Plotting serial and OpenMP execution times
# ------------------------------	

plt.figure(figsize=(10, 6))

for i, config in enumerate(configurations):
    plt.plot(['Serial'] + ['OMP{}'.format(threads[j]) for j in range(len(threads))], [serial_times[i]] + openmp_times[i], marker='o', label=config)

plt.title('Execution Times vs Threads')
plt.xlabel('Configuration')
plt.ylabel('Execution Time (s)')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig('imgs/execution_times.png')  # Save the figure as PNG
plt.show()


# ------------------------------
# Plotting speedup
# ------------------------------

plt.figure(figsize=(10, 6))

for i in range(len(configurations)):
    plt.plot(threads, speedup[i], marker='o', label=configurations[i])

plt.title('Speedup vs Threads')
plt.xlabel('Threads')
plt.ylabel('Speedup')
plt.xticks(threads)
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig('imgs/speedup_vs_threads.png')  # Save the figure as PNG
plt.show()

