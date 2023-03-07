#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define raz 1000

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s max_num_iter max_toch raz\n", argv[0]);
        return 1;
    }

    const int max_num_iter = atoi(argv[1]);
    const double max_toch = atof(argv[2]);
    const int n = atoi(argv[3]);

    double arr_pred[raz][raz] = {0.0};
    double arr_new[raz][raz] = {0.0};

    arr_pred[0][0] = 10;
    arr_pred[0][n - 1] = 20;
    arr_pred[n - 1][n - 1] = 20;
    arr_pred[n - 1][0] = 30;

    const clock_t start_time = clock();

    double error = 0.0;
    int num_iter = 0;

    while (num_iter < max_num_iter && error > max_toch) {
        error = 0.0;

#pragma omp parallel for reduction(max : error)
        for (int j = 1; j < n - 1; ++j) {
            for (int i = 1; i < n - 1; ++i) {
                arr_new[i][j] = (arr_pred[i - 1][j] + arr_pred[i][j - 1] + arr_pred[i][j + 1] + arr_pred[i + 1][j]) * 0.25;
                error = fmax(fabs(arr_pred[i][j] - arr_new[i][j]), error);
            }
        }

#pragma omp parallel for
        for (int j = 1; j < n - 1; ++j) {
            for (int i = 1; i < n - 1; ++i) {
                arr_pred[j][i] = arr_new[j][i];
            }
        }

        if (num_iter % 10 == 0) {
            printf("Iteration: %d, error: %0.8lf\n", num_iter, error);
        }

        ++num_iter;
    }

    const clock_t end_time = clock();
    const double elapsed_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC * 1000;

    printf("Elapsed time: %0.2lf ms\n", elapsed_time);

    return 0;
}
