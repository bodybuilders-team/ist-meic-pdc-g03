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

void print_grid(char ***grid, int32_t N, int start_x, int end_x)
{
    printf("Start_x: %d, End_x: %d\n", start_x, end_x);
    for (int32_t x = start_x; x < end_x; x++)
    {
        printf("Layer %d:\n", x);
        for (int32_t y = 0; y < N; y++)
        {
            for (int32_t z = 0; z < N; z++)
            {
                if (grid[x - start_x][y][z] == 0)
                    printf(". ");
                else
                    printf("%d ", grid[x - start_x][y][z]);
            }
            printf("\n");
        }
        printf("\n");
    }
}
