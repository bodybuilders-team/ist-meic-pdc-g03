#include <stdio.h>
#include "constants.h"

/**
 * @brief Print the result for each species in increasing order
 *
 * @param max_counts Array of maximum counts for each species
 * @param max_generations Array of maximum generations for each species
 */
void print_result(long max_counts[N_SPECIES + 1], int max_generations[N_SPECIES + 1])
{
    for (int species = 1; species <= N_SPECIES; species++)
    {
        printf("%d %ld %d\n", species, max_counts[species], max_generations[species]);
    }
}

/**
 * @brief Print the grid
 *
 * @param grid 3D grid
 * @param N Size of the grid
 */
void print_grid(char ***grid, int N)
{
    for (int x = 0; x < N; x++)
    {
        printf("Layer %d:\n", x);
        for (int y = 0; y < N; y++)
        {
            for (int z = 0; z < N; z++)
            {
                if (grid[x][y][z] == 0)
                    printf(". ");
                else
                    printf("%d ", grid[x][y][z]);
            }
            printf("\n");
        }
        printf("\n");
    }
}