#include <stdio.h>
#include "constants.h"
#include "stdint.h"

void print_result(uint64_t max_counts[N_SPECIES + 1], int max_generations[N_SPECIES + 1])
{
    for (uint8_t species = 1; species <= N_SPECIES; species++)
    {
        printf("%d %ld %d\n", species, max_counts[species], max_generations[species]);
    }
}

void print_grid(char ***grid, uint32_t N)
{
    for (uint32_t x = 0; x < N; x++)
    {
        printf("Layer %d:\n", x);
        for (uint32_t y = 0; y < N; y++)
        {
            for (uint32_t z = 0; z < N; z++)
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