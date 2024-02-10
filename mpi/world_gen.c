#include <stdio.h>

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

/**
 * @brief Generates an initial grid for the simulation.
 *
 * This function generates an initial grid for the simulation with a given size and density.
 * The grid is a 3D matrix of size `N x N x N` where each cell contains a value between 0 and 9.
 * The density parameter is used to determine the probability of a cell being occupied by a species.
 *
 * @param N The size of the grid.
 * @param density The probability of a cell being occupied by a species.
 * @param input_seed The seed value used to initialize the random number generator.
 * @return A 3D matrix representing the initial grid for the simulation. e.g. `grid[x][y][z]`
 */
char ***gen_initial_grid(long long N, float density, int input_seed)
{
    int x, y, z;

    grid = (char ***)malloc(N * sizeof(char **));
    if (grid == NULL)
    {
        printf("Failed to allocate matrix\n");
        exit(1);
    }
    for (x = 0; x < N; x++)
    {
        grid[x] = (char **)malloc(N * sizeof(char *));
        if (grid[x] == NULL)
        {
            printf("Failed to allocate matrix\n");
            exit(1);
        }
        grid[x][0] = (char *)calloc(N * N, sizeof(char));
        if (grid[x][0] == NULL)
        {
            printf("Failed to allocate matrix\n");
            exit(1);
        }
        for (y = 1; y < N; y++)
            grid[x][y] = grid[x][0] + y * N;
    }

    init_r4uni(input_seed);
    for (x = 0; x < N; x++)
        for (y = 0; y < N; y++)
            for (z = 0; z < N; z++)
                if (r4_uni() < density)
                    grid[x][y][z] = (int)(r4_uni() * N_SPECIES) + 1;

    return grid;
}
