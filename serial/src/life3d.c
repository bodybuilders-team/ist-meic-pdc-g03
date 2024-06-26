#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <omp.h>

#include "constants.h"
#include "grid.h"

// To run: make clean && make && bin/life3d 4 4 .4 100

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
    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s <number of generations> <number of cells per side of the cube> <density of the initial population> <seed for the random number generator>\n", argv[0]);
        return 1;
    }

    int32_t num_generations = atoi(argv[1]);
    int32_t num_cells = atoi(argv[2]);
    double density = atof(argv[3]);
    int seed = atoi(argv[4]);

    double exec_time;

    // Generate initial grid
    char ***grid = gen_initial_grid(num_cells, density, seed);

    int64_t *max_counts = (int64_t *)calloc(N_SPECIES + 1, sizeof(long));
    int32_t *max_generations = (int32_t *)calloc(N_SPECIES + 1, sizeof(int));

    exec_time = -omp_get_wtime();

    // Run the simulation
    simulation(grid, num_cells, max_counts, max_generations, num_generations);

    exec_time += omp_get_wtime();

    // Print execution time
    fprintf(stderr, "%.1f\n", exec_time);

    // Print the result to stdout
    print_result(max_counts, max_generations);

    return 0;
}
