#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "constants.h"
#include "grid.h"

void simulation(char ***grid, int N, long *max_counts, int *max_generations, int num_generations)
{
    // Temporary grid to hold the next generation
    char ***next_grid = (char ***)malloc(N * sizeof(char **));
    if (next_grid == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for next_grid\n");
        exit(1);
    }

    for (int x = 0; x < N; x++)
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
        for (int y = 1; y < N; y++)
            next_grid[x][y] = next_grid[x][0] + y * N;
    }
    
    // Pointers to the grids for buffer swapping
    char ***current_grid = grid;
    char ***current_next_grid = next_grid;

    // Print for debugging - Initial grid (generation 0)
    //printf("Generation 0    ------------------------------\n");
    //print_grid(grid, N);

    long initial_species_counts[N_SPECIES + 1] = {0};

    #pragma omp parallel for collapse(3) shared(grid, N, initial_species_counts)
    for (int x = 0; x < N; x++)
    {
        for (int y = 0; y < N; y++)
        {
            for (int z = 0; z < N; z++)
            {
                if (grid[x][y][z] > 0)
                    #pragma omp atomic
                    initial_species_counts[(int)grid[x][y][z]]++;
            }
        }
    }

    // Update the maximum counts and generations
    #pragma omp parallel for shared(max_counts, max_generations, initial_species_counts)
    for (int s = 1; s <= N_SPECIES; s++)
    {
        if (initial_species_counts[s] > max_counts[s])
        {
            max_counts[s] = initial_species_counts[s];
            max_generations[s] = 0;
        }
    }

    // Perform simulation for the specified number of generations
    for (int gen = 1; gen <= num_generations; gen++)
    {
        long species_counts[N_SPECIES + 1] = {0};

        // Iterate over each cell in the grid
        #pragma omp parallel for collapse(3) shared(current_grid, current_next_grid, N) reduction(+:species_counts[:N_SPECIES+1])
        for (int x = 0; x < N; x++)
        {
            for (int y = 0; y < N; y++)
            {
                for (int z = 0; z < N; z++)
                {
                    // Count the number of live neighbors for the current cell
                    int count_neighbors = 0;
                    // Determine the majority species of the neighbors
                    int neighbor_species_counts[N_SPECIES + 1] = {0};
                    
                    for (int i = -1; i <= 1; i++)
                    {
                        for (int j = -1; j <= 1; j++)
                        {
                            for (int k = -1; k <= 1; k++)
                            {
                                if (i == 0 && j == 0 && k == 0)
                                    continue; // Skip the current cell
                                int nx = (x + i + N) % N;
                                int ny = (y + j + N) % N;
                                int nz = (z + k + N) % N;
                                if (nx >= 0 && nx < N && ny >= 0 && ny < N && nz >= 0 && nz < N)
                                {
                                    int species = current_grid[nx][ny][nz];
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
                    if (current_grid[x][y][z] > 0)
                    {
                        if (count_neighbors <= 4 || count_neighbors > 13)
                            current_next_grid[x][y][z] = 0; // Cell dies
                        else
                        {
                            current_next_grid[x][y][z] = current_grid[x][y][z]; // Cell survives
                            // #pragma omp atomic
                            species_counts[(int)current_grid[x][y][z]]++;
                        }
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
                            current_next_grid[x][y][z] = max_species; // Cell becomes alive with majority 
                            // #pragma omp atomic
                            species_counts[max_species]++;
                        }
                        else
                        {
                            current_next_grid[x][y][z] = 0; // Cell remains dead
                        }
                    }
                }
            }
        }

        // Count the number of each species
        // #pragma omp parallel for collapse(3) shared(current_next_grid, N) reduction(+:species_counts[:N_SPECIES+1])
        // for (int x = 0; x < N; x++)
        // {
        //     for (int y = 0; y < N; y++)
        //     {
        //         for (int z = 0; z < N; z++)
        //         {
        //             if (current_next_grid[x][y][z] > 0)
        //             {
        //                 // #pragma omp atomic
        //                 species_counts[(int)current_next_grid[x][y][z]]++;
        //             }
        //         }
        //     }
        // }

        // Swap the current grid with the next grid
        char ***temp = current_grid;
        current_grid = current_next_grid;
        current_next_grid = temp;

        // Update the maximum counts and generations
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
    for (int x = 0; x < N; x++)
    {
        free(next_grid[x][0]);
        free(next_grid[x]);
    }
    free(next_grid);
}
