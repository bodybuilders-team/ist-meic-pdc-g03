#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <omp.h>
#include <mpi.h>
#include "constants.h"
#include "grid.h"

/**
 * @brief Entry point of the program.
 *
 * Receives the input parameters and runs the simulation.
 *
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 * @return 0 if the program exits successfully, 1 otherwise.
 */
int main(int argc, char *argv[])
{
    // Initialize MPI
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s <number of generations> <number of cells per side of the cube> <density of the initial population> <seed for the random number generator>\n", argv[0]);
        return 1;
    }

    uint32_t num_generations = atoi(argv[1]);
    uint32_t num_cells = atoi(argv[2]);
    double density = atof(argv[3]);
    int seed = atoi(argv[4]);

    double exec_time;

    // Generate initial grid
    char ***grid = gen_initial_grid(num_cells, density, seed);

    uint64_t *max_counts = (uint64_t *)calloc(N_SPECIES + 1, sizeof(long));
    uint32_t *max_generations = (uint32_t *)calloc(N_SPECIES + 1, sizeof(int));

    exec_time = -omp_get_wtime();

    // Run the simulation
    simulation(grid, num_cells, max_counts, max_generations, num_generations);

    exec_time += omp_get_wtime();

    // Print execution time
    fprintf(stderr, "%.1fs\n", exec_time);

    // Print the result to stdout
    print_result(max_counts, max_generations);

    MPI_Finalize();
    return 0;
}
