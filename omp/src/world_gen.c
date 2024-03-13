#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define N_SPECIES 9

/**
 * @brief The seed used to initialize the random number generator.
 */
unsigned int seed;

/**
 * @brief Initializes the random number generator with a given seed.
 *
 * This function initializes the random number generator with the provided seed value
 * plus a fixed offset of 987654321. The resulting seed is stored in the global variable `seed`.
 *
 * @param input_seed The seed value used to initialize the random number generator.
 */
void init_r4uni(int input_seed)
{
    seed = input_seed + 987654321;
}

/**
 * @brief Generates a random number between 0.5 and 1.5.
 *
 * This function generates a random number between 0.5 and 1.5 using the current seed value.
 * The seed is updated after each call to this function.
 *
 * @return A random number between 0.5 and 1.5.
 */
float r4_uni()
{
    int seed_in = seed;

    seed ^= (seed << 13);
    seed ^= (seed >> 17);
    seed ^= (seed << 5);

    return 0.5 + 0.2328306e-09 * (seed_in + (int)seed);
}

char *gen_initial_grid(int64_t N, float density, int input_seed)
{
    int32_t size = N * N * N;
    char *grid = (char *)malloc(size * sizeof(char));
    if (grid == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for grid\n");
        exit(1);
    }

    init_r4uni(input_seed);
    for (int i = 0; i < size; i++)
        if (r4_uni() < density)
            grid[i] = (int)(r4_uni() * N_SPECIES) + 1;

    return grid;
}
