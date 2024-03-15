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

/**
 * @brief Generates the initial partial grid for the simulation
 *
 * This function generates an initial grid for the simulation with a given size and density.
 * The grid is a 3D matrix of size `N x N x N` where each cell contains a value between 0 and 9.
 * The density parameter is used to determine the probability of a cell being occupied by a species.
 *
 * A partial grid is generated for the specified range of x-coordinates, so that each MPI process owns a portion of the grid (a number of layers).
 * Two additional layers are added to the grid to store the state of the neighboring cells, that are required to calculate the next generation.
 * These layers are added to the beginning and end of the grid, so that the first and last layers of the grid are used to store the state of the neighboring cells,
 * calculated by the neighboring MPI processes.
 *
 * @param N The size of the grid.
 * @param density The probability of a cell being occupied by a species.
 * @param input_seed The seed value used to initialize the random number generator.
 * @param start_x The starting x-coordinate for the grid.
 * @param end_x The ending x-coordinate for the grid.
 * @return A 3D matrix representing the initial grid for the simulation. e.g. `grid[x][y][z]`
 */
char ***gen_initial_grid_partial(int64_t N, float density, int input_seed, int start_x, int end_x)
{
    int32_t x, y, z;

    char ***grid = (char ***)malloc((end_x - start_x + 2) * sizeof(char **));
    if (grid == NULL)
    {
        printf("Failed to allocate matrix\n");
        exit(1);
    }
    for (x = start_x - 1; x < end_x + 1; x++)
    {
        int32_t nx = (x + N) % N;

        grid[nx - start_x + 1] = (char **)malloc(N * sizeof(char *));
        if (grid[nx - start_x + 1] == NULL)
        {
            printf("Failed to allocate matrix\n");
            exit(1);
        }
        grid[nx - start_x][0] = (char *)calloc(N * N, sizeof(char));
        if (grid[nx - start_x][0] == NULL)
        {
            printf("Failed to allocate matrix\n");
            exit(1);
        }
        for (y = 1; y < N; y++)
            grid[nx - start_x][y] = grid[nx - start_x][0] + y * N;
    }

    init_r4uni(input_seed);

    // Generate other layers and ignore to synchronize the random number generator
    for (x = 0; x < start_x - 1; x++)
        for (y = 0; y < N; y++)
            for (z = 0; z < N; z++)
                if (r4_uni() < density)
                    r4_uni();

    for (x = start_x - 1; x < end_x + 1; x++)
        for (y = 0; y < N; y++)
            for (z = 0; z < N; z++)
                if (r4_uni() < density)
                {
                    int nx = (x + N) % N;
                    grid[nx - start_x][y][z] = (int)(r4_uni() * N_SPECIES) + 1;
                }

    return grid;
}
