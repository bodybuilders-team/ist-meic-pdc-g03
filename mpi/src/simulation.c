#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <omp.h>
#include <mpi.h>

#include "constants.h"
#include "grid.h"

void simulation(char ***grid, int32_t N, int64_t *max_counts, int32_t *max_generations, int32_t num_generations, int32_t start_x, int32_t end_x, int rank, int size)
{
    int32_t my_n = end_x - start_x;
    MPI_Request requests[4];

    // Temporary grid to hold the next generation
    char ***next_grid = (char ***)malloc((my_n + 2) * sizeof(char **));
    if (next_grid == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for next_grid\n");
        exit(1);
    }

    for (int32_t x = 0; x < my_n + 2; x++)
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
        for (int32_t y = 1; y < N; y++)
            next_grid[x][y] = next_grid[x][0] + y * N;
    }

    // Print for debugging - Initial grid (generation 0)
    // printf("Generation 0    ------------------------------\n");
    // print_grid(grid, N, start_x, end_x);

    int64_t initial_species_counts[N_SPECIES + 1] = {0};

    #pragma omp parallel for shared(grid, N) reduction(+:initial_species_counts[:N_SPECIES + 1])
    for (int32_t x = 1; x < my_n + 1; x++)
    {
        for (int32_t y = 0; y < N; y++)
        {
            for (int32_t z = 0; z < N; z++)
            {
                int16_t species = grid[x][y][z];
                if (species > 0)
                    initial_species_counts[species]++;
            }
        }
    }

    // Perform global reduction for initial_species_counts
    MPI_Reduce(initial_species_counts, max_counts, N_SPECIES + 1, MPI_INT64_T, MPI_SUM, 0, MPI_COMM_WORLD);
    for (int16_t s = 1; s <= N_SPECIES; s++){
        max_generations[s] = 0;
    }

    // To iterate over the cells of my grid, I need my neighbors, but we can start calculating the state for other layers
    // in the meantime. So we can start in our second layer if we have more than one layer.
    // This just happens if we have more than 2 layers, because we need to calculate the first and last layers in the end.
    int start_x_gen = my_n > 1 ? 2 : 1;
    int end_x_gen = my_n > 1 ? my_n + 2 : my_n + 1;

    // Perform simulation for the specified number of generations
    for (int32_t gen = 1; gen <= num_generations; gen++)
    {
        int64_t species_counts[N_SPECIES + 1] = {0};

        // Iterate over each cell in the grid
        #pragma omp parallel for shared(grid, next_grid, N) reduction(+:species_counts[:N_SPECIES + 1])
        for (int32_t a = start_x_gen; a < end_x_gen; a++)
        {
            // Wait for the ghost layers to be received to calculate the first and last layers
            if (gen > 1 && a >= my_n)
                MPI_Waitall(4, requests, MPI_STATUSES_IGNORE);
            
            int x = a;
            if (x == my_n + 1)
                x = 1; // We calculate the first layer at the end

            for (int32_t y = 0; y < N; y++)
            {
                for (int32_t z = 0; z < N; z++)
                {
                    // Count the number of live neighbors for the current cell
                    int16_t alive_neighbors = 0;
                    // Determine the majority species of the neighbors
                    int16_t neighbor_species_counts[N_SPECIES + 1] = {0};

                    for (int8_t i = -1; i <= 1; i++)
                    {
                        int32_t nx = x + i;

                        for (int8_t j = -1; j <= 1; j++)
                        {
                            int32_t ny = y + j;
                            ny = ny >= N ? 0 : ny < 0 ? N - 1
                                                      : ny;

                            for (int8_t k = -1; k <= 1; k++)
                            {
                                if (i == 0 && j == 0 && k == 0)
                                    continue; // Skip the current cell

                                int32_t nz = z + k;
                                nz = nz >= N ? 0 : nz < 0 ? N - 1
                                                          : nz;

                                int16_t species = grid[nx][ny][nz];
                                if (species > 0)
                                {
                                    alive_neighbors++;
                                    neighbor_species_counts[species]++;
                                }
                            }
                        }
                    }

                    int16_t current_species = grid[x][y][z];

                    // Apply the rules of the Game of Life
                    if (current_species > 0)
                    {
                        if (alive_neighbors <= 4 || alive_neighbors > 13)
                            next_grid[x][y][z] = 0; // Cell dies
                        else
                        {
                            next_grid[x][y][z] = current_species; // Cell survives
                            species_counts[current_species]++;
                        }
                    }
                    else
                    {
                        if (alive_neighbors >= 7 && alive_neighbors <= 10)
                        {
                            // Determine the majority species
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
                            next_grid[x][y][z] = max_species; // Cell becomes alive with majority
                            species_counts[max_species]++;
                        }
                        else
                        {
                            next_grid[x][y][z] = 0; // Cell remains dead
                        }
                    }
                }
            }
        }

        // Update the maximum counts and generations
        int64_t total_species_counts[N_SPECIES + 1] = {0};
        MPI_Reduce(species_counts, total_species_counts, N_SPECIES + 1, MPI_INT64_T, MPI_SUM, 0, MPI_COMM_WORLD);
        for (int16_t s = 1; s <= N_SPECIES; s++)
        {
            if (total_species_counts[s] > max_counts[s])
            {
                max_counts[s] = total_species_counts[s];
                max_generations[s] = gen;
            }
        }

        // Swap the current grid with the next grid
        char ***temp = grid;
        grid = next_grid;
        next_grid = temp;

        // Update my ghost layers and send my first and last layer to my neighbors
        int send_to = rank == size - 1 ? 0 : rank + 1;
        int recv_from = rank == 0 ? size - 1 : rank - 1;
        int first_tag = 3 * gen;

        MPI_Irecv(grid[0][0], N * N, MPI_CHAR, recv_from, first_tag, MPI_COMM_WORLD, &requests[0]);
        MPI_Isend(grid[my_n][0], N * N, MPI_CHAR, send_to, first_tag, MPI_COMM_WORLD, &requests[1]);

        int last_tag = 3 * gen + 1;
        MPI_Irecv(grid[my_n + 1][0], N * N, MPI_CHAR, send_to, last_tag, MPI_COMM_WORLD, &requests[2]);
        MPI_Isend(grid[1][0], N * N, MPI_CHAR, recv_from, last_tag, MPI_COMM_WORLD, &requests[3]);

        //MPI_Waitall(4, requests, MPI_STATUSES_IGNORE);

        // Print for debugging
        // printf("Generation %d    ------------------------------\n", gen);
        // print_grid(grid, N, start_x, end_x);
    }
}
