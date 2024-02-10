void print_result() {
    // Array to store the maximum count of each species and the generation where it occurred
    int max_counts[N_SPECIES + 1] = {0};
    int max_generations[N_SPECIES + 1] = {0};

    // Iterate over each cell in the grid to find the maximum count of each species
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            for (int z = 0; z < N; z++) {
                int species = grid[x][y][z];
                if (species > 0) {
                    // Increment the count for the current species
                    max_counts[species]++;
                    // Update the generation where the maximum count occurred
                    if (max_generations[species] == 0 || max_counts[species] > max_counts[species]) {
                        max_generations[species] = z; // Assuming z represents the generation
                    }
                }
            }
        }
    }

    // Print the result for each species in increasing order
    for (int species = 1; species <= N_SPECIES; species++) {
        printf("%d %d %d\n", species, max_counts[species], max_generations[species]);
    }
}
