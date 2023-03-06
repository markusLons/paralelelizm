#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ARRAY_SIZE 1024
#define MAX_ITERATIONS 1000000
#define TOLERANCE 1e-6

int main() {
    double** old_array = malloc(ARRAY_SIZE * sizeof(double*));
    double** new_array = malloc(ARRAY_SIZE * sizeof(double*));

    for (int i = 0; i < ARRAY_SIZE; i++) {
        old_array[i] = calloc(ARRAY_SIZE, sizeof(double));
        new_array[i] = calloc(ARRAY_SIZE, sizeof(double));
    }

    // Set boundary conditions
    old_array[0][0] = 10;
    old_array[0][ARRAY_SIZE-1] = 20;
    old_array[ARRAY_SIZE-1][0] = 30;
    old_array[ARRAY_SIZE-1][ARRAY_SIZE-1] = 20;

    // Perform iterative method
    int iter = 0;
    double err = TOLERANCE + 1;

    while (err > TOLERANCE && iter < MAX_ITERATIONS) {
        err = 0;

        // Update internal points
#pragma omp parallel for reduction(max:err)
        for (int j = 1; j < ARRAY_SIZE - 1; j++) {
            for (int i = 1; i < ARRAY_SIZE - 1; i++) {
                new_array[i][j] = 0.25 * (old_array[i+1][j] + old_array[i-1][j]
                                          + old_array[i][j-1] + old_array[i][j+1]);
                err = fmax(err, fabs(new_array[i][j] - old_array[i][j]));
            }
        }

        // Swap arrays
        double** temp = old_array;
        old_array = new_array;
        new_array = temp;

        iter++;

        if (iter % 100 == 0) {
            printf("%d, %0.6lf\n", iter, err);
        }
    }

    printf("result: %d, %0.6lf\n", iter, err);

    // Free memory
    for (int i = 0; i < ARRAY_SIZE; i++) {
        free(old_array[i]);
        free(new_array[i]);
    }
    free(old_array);
    free(new_array);

    return 0;
}
