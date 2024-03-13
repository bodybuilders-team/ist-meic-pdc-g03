#include <stdio.h>
#include <stdint.h>

#include "constants.h"

void print_result(int64_t max_counts[N_SPECIES + 1], int max_generations[N_SPECIES + 1])
{
    for (int16_t species = 1; species <= N_SPECIES; species++)
    {
        printf("%d %ld %d\n", species, max_counts[species], max_generations[species]);
    }
}

void print_grid(char *grid, int32_t N)
{
    for (int32_t x = 0; x < N; x++)
    {
        printf("Layer %d:\n", x);
        for (int32_t y = 0; y < N; y++)
        {
            for (int32_t z = 0; z < N; z++)
            {
                int32_t index = x * N * N + y * N + z;
                if (grid[index] == 0)
                    printf(". ");                else
                    printf("%d ", grid[index]);
            }
            printf("\n");
        }
        printf("\n");
    }
}