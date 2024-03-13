#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <omp.h>
#include "constants.h"
#include "grid.h"

void simulation(char *grid, int32_t N, int64_t *max_counts, int32_t *max_generations, int32_t num_generations)
{
    int32_t size = N * N * N;
    char *next_grid = (char *)malloc(size * sizeof(char));
    if (next_grid == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for next_grid\n");
        exit(1);
    }

    int64_t initial_species_counts[N_SPECIES + 1] = {0};
    #pragma omp parallel for shared(grid, N) reduction(+:initial_species_counts[:N_SPECIES + 1])
    for (int32_t i = 0; i < size; i++)
    {
        int16_t species = grid[i];
        if (species > 0)
            initial_species_counts[species]++;
    }

    for (int16_t s = 1; s <= N_SPECIES; s++)
    {
        if (initial_species_counts[s] > max_counts[s])
        {
            max_counts[s] = initial_species_counts[s];
            max_generations[s] = 0;
        }
    }

    int32_t neighbor_offsets[26] = {
        -N*N-N-1, -N*N-N, -N*N-N+1, -N*N-1, -N*N, -N*N+1, -N*N+N-1, -N*N+N, -N*N+N+1,
        -N-1, -N, -N+1, -1, 1, N-1, N, N+1,
        N*N-N-1, N*N-N, N*N-N+1, N*N-1, N*N, N*N+1, N*N+N-1, N*N+N, N*N+N+1
    };

    for (int32_t gen = 1; gen <= num_generations; gen++)
    {
        int64_t species_counts[N_SPECIES + 1] = {0};
        #pragma omp parallel for shared(grid, N) reduction(+:species_counts[:N_SPECIES + 1])
        for (int32_t i = 0; i < size; i++)
        {
            int16_t alive_neighbors = 0;
            int16_t neighbor_species_counts[N_SPECIES + 1] = {0};
            for (int8_t j = 0; j < 26; j++)
            {
                int32_t neighbor_index = i + neighbor_offsets[j];
                if (neighbor_index >= 0 && neighbor_index < size)
                {
                    int16_t species = grid[neighbor_index];
                    if (species > 0)
                    {
                        alive_neighbors++;
                        neighbor_species_counts[species]++;
                    }
                }
            }

            int16_t current_species = grid[i];
            if (current_species > 0)
            {
                if (alive_neighbors <= 4 || alive_neighbors > 13)
                    next_grid[i] = 0;
                else
                {
                    next_grid[i] = current_species;
                    species_counts[current_species]++;
                }
            }
            else
            {
                if (alive_neighbors >= 7 && alive_neighbors <= 10)
                {
                    int16_t max_species = 0;
                    int16_t max_count = 0;
                    for (int16_t s = 1; s <= N_SPECIES; s++)
                    {
                        if (neighbor_species_counts[s] > max_count)
                        {
                            max_species = s;
                            max_count = neighbor_species_counts[s];
                        }
                    }
                    next_grid[i] = max_species;
                    species_counts[max_species]++;
                }
                else
                {
                    next_grid[i] = 0;
                }
            }
        }

        char *temp = grid;
        grid = next_grid;
        next_grid = temp;

        for (int16_t s = 1; s <= N_SPECIES; s++)
        {
            if (species_counts[s] > max_counts[s])
            {
                max_counts[s] = species_counts[s];
                max_generations[s] = gen;
            }
        }
    }

    free(next_grid);
}