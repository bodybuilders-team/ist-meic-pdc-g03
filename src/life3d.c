#include <stdio.h>
#include <omp.h>

// Function to generate initial grid
void gen_initial_grid(int N, int seed, double density);

// Function to run the simulation
void simulation();

// Function to print the result
void print_result();

int main(int argc, char *argv[]) {
    double exec_time;
    int N = 100; // Example value for N
    int seed = 123; // Example seed value
    double density = 0.5; // Example density value

    // Generate initial grid
    gen_initial_grid(N, seed, density);

    exec_time = -omp_get_wtime();

    // Run the simulation
    simulation();

    exec_time += omp_get_wtime();

    // Print execution time
    fprintf(stderr, "%.1fs\n", exec_time);

    // Print the result to stdout
    print_result();

    return 0;
}
