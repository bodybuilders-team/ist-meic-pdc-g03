#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "constants.h"
#include "grid.h"

void simulation(char ***grid, int N, long *max_counts, int *max_generations, int num_generations)
{
    int x, y, z, i, j, k;
    int count_neighbors;

    // Temporary grid to hold the next generation
    char ***next_grid = (char ***)malloc(N * sizeof(char **));
    if (next_grid == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for next_grid\n");
        exit(1);
    }

    for (x = 0; x < N; x++)
    {
        next_grid[x] = (char **)malloc(N * sizeof(char *));
        if (next_grid[x] == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for next_grid\n");
            exit(1);
        }
        next_grid[x][0] = (char *)calloc(N * N, sizeof(char));
        if (next_grid[x][0] == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for next_grid\n");
            exit(1);
        }
        for (y = 1; y < N; y++)
            next_grid[x][y] = next_grid[x][0] + y * N;
    }
    

    // Print for debugging - Initial grid (generation 0)
    //printf("Generation 0    ------------------------------\n");
    //print_grid(grid, N);

    long species_counts[N_SPECIES + 1] = {0};

    #pragma omp parallel for collapse(3) private(x, y, z) shared(grid, N, species_counts)
    for (x = 0; x < N; x++)
    {
        for (y = 0; y < N; y++)
        {
            for (z = 0; z < N; z++)
            {
                if (grid[x][y][z] > 0)
                    #pragma omp atomic
                    species_counts[(int)grid[x][y][z]]++;
            }
        }
    }

    // Update the maximum counts and generations
    #pragma omp parallel for shared(max_counts, max_generations, species_counts)
    for (int s = 1; s <= N_SPECIES; s++)
    {
        if (species_counts[s] > max_counts[s])
        {
            max_counts[s] = species_counts[s];
            max_generations[s] = 0;
        }
    }

    // Perform simulation for the specified number of generations
    for (int gen = 1; gen <= num_generations; gen++)
    {
        // Iterate over each cell in the grid
        #pragma omp parallel for collapse(3) private(x, y, z, count_neighbors) shared(grid, next_grid, N)
        for (x = 0; x < N; x++)
        {
            for (y = 0; y < N; y++)
            {
                for (z = 0; z < N; z++)
                {
                    // Count the number of live neighbors for the current cell
                    count_neighbors = 0;
                    // Determine the majority species of the neighbors
                    int neighbor_species_counts[N_SPECIES + 1] = {0};
                    
                    for (i = -1; i <= 1; i++)
                    {
                        for (j = -1; j <= 1; j++)
                        {
                            for (k = -1; k <= 1; k++)
                            {
                                if (i == 0 && j == 0 && k == 0)
                                    continue; // Skip the current cell
                                int nx = (x + i + N) % N;
                                int ny = (y + j + N) % N;
                                int nz = (z + k + N) % N;
                                if (nx >= 0 && nx < N && ny >= 0 && ny < N && nz >= 0 && nz < N)
                                {
                                    int species = grid[nx][ny][nz];
                                    if (species > 0)
                                    {
                                        count_neighbors++;
                                        neighbor_species_counts[species]++;
                                    }
                                }
                            }
                        }
                    }

                    // Apply the rules of the Game of Life
                    if (grid[x][y][z] > 0)
                    {
                        if (count_neighbors <= 4 || count_neighbors > 13)
                            next_grid[x][y][z] = 0; // Cell dies
                        else
                            next_grid[x][y][z] = grid[x][y][z]; // Cell survives
                    }
                    else
                    {
                        if (count_neighbors >= 7 && count_neighbors <= 10)
                        {
                            // Determine the majority species
                            int max_species = 0;
                            int max_count = 0;
                            for (int s = 1; s <= N_SPECIES; s++)
                            {
                                if (neighbor_species_counts[s] > max_count)
                                {
                                    max_species = s;
                                    max_count = neighbor_species_counts[s];
                                }
                            }
                            next_grid[x][y][z] = max_species; // Cell becomes alive with majority species
                        }
                        else
                        {
                            next_grid[x][y][z] = 0; // Cell remains dead
                        }
                    }
                }
            }
        }

        long species_counts[N_SPECIES + 1] = {0};

        // Update the grid with the next generation and count the number of each species
        #pragma omp parallel for collapse(3) private(x, y, z) shared(grid, next_grid, N, species_counts)
        for (x = 0; x < N; x++)
        {
            for (y = 0; y < N; y++)
            {
                for (z = 0; z < N; z++)
                {
                    grid[x][y][z] = next_grid[x][y][z];

                    if (grid[x][y][z] > 0)
                        #pragma omp atomic
                        species_counts[(int)grid[x][y][z]]++;
                }
            }
        }

        // Update the maximum counts and generations
        #pragma omp parallel for shared(max_counts, max_generations, species_counts)
        for (int s = 1; s <= N_SPECIES; s++)
        {
            if (species_counts[s] > max_counts[s])
            {
                max_counts[s] = species_counts[s];
                max_generations[s] = gen;
            }
        }

        // Print for debugging
        //printf("Generation %d    ------------------------------\n", gen);
        //print_grid(grid, N);
    }

    // Free memory for next_grid
    for (x = 0; x < N; x++)
    {
        free(next_grid[x][0]);
        free(next_grid[x]);
    }
    free(next_grid);
}
