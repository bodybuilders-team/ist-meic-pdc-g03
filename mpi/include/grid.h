#pragma once

#include "constants.h"
#include "stdint.h"

/**
 * @brief Generates the initial partial grid for the simulation
 *
 * This function generates an initial grid for the simulation with a given size and density.
 * The grid is a 3D matrix of size `N x N x N` where each cell contains a value between 0 and 9.
 * The density parameter is used to determine the probability of a cell being occupied by a species.
 * 
 * A partial grid is generated for the specified range of x-coordinates, so that each MPI process owns a portion of the grid (a number of layers).
 * Two additional layers are added to the grid to store the state of the neighboring cells, that are required to calculate the next generation.
 * These layers are added to the beginning and end of the grid, so that the first and last layers of the grid are used to store the state of the neighboring cells,
 * calculated by the neighboring MPI processes.
 *
 * @param N The size of the grid.
 * @param density The probability of a cell being occupied by a species.
 * @param input_seed The seed value used to initialize the random number generator.
 * @param start_x The starting x-coordinate for the grid.
 * @param end_x The ending x-coordinate for the grid.
 * @return A 3D matrix representing the initial grid for the simulation. e.g. `grid[x][y][z]`
 */
char ***gen_initial_grid_partial(int64_t N, float density, int input_seed, int start_x, int end_x);

/**
 * @brief Perform the simulation for the specified number of generations
 *
 * @param grid 3D grid
 * @param N Size of the grid
 * @param max_counts Array of maximum counts for each species
 * @param max_generations Array of maximum generations for each species
 * @param num_generations Number of generations to simulate
 * @param start_x The starting x-coordinate for the grid.
 * @param end_x The ending x-coordinate for the grid.
 * @param rank The rank of the MPI process
 * @param size The total number of MPI processes
 */
void simulation(char ***grid, int32_t N, int64_t *max_counts, int32_t *max_generations, int32_t num_generations, int start_x, int end_x, int rank, int size);

/**
 * @brief Print the result for each species in increasing order
 *
 * @param max_counts Array of maximum counts for each species
 * @param max_generations Array of maximum generations for each species
 */
void print_result(int64_t max_counts[N_SPECIES + 1], int32_t max_generations[N_SPECIES + 1]);

/**
 * @brief Print the grid
 *
 * @param grid 3D grid
 * @param N Size of the grid
 * @param start_x The starting x-coordinate for the grid.
 * @param end_x The ending x-coordinate for the grid.
 */
void print_grid(char ***grid, int32_t N, int start_x, int end_x);