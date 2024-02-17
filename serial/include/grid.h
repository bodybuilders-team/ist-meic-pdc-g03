#pragma once

#include "constants.h"

// Function declarations
char ***gen_initial_grid(long long N, float density, int input_seed);
void simulation(char ***grid, int N, long *max_counts, int *max_generations, int num_generations);
void print_result(long max_counts[N_SPECIES + 1], int max_generations[N_SPECIES + 1]);

void print_grid(char ***grid, int N);