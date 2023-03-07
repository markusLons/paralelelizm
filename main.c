#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <time.h>

#define raz 1000

__global__ void jacobi_kernel(double* arr_new, double* arr_pred, double* error, int n) {
    double max_error = 0.0;
    for (int j = 1; j < n - 1; ++j) {
        for (int i = 1; i < n - 1; ++i) {
            arr_new[i + j * n] = (arr_pred[i - 1 + j * n] + arr_pred[i + (j - 1) * n] + arr_pred[i + (j + 1) * n] + arr_pred[i + 1 + j * n]) * 0.25;
            double diff = fabs(arr_pred[i + j * n] - arr_new[i + j * n]);
            max_error = fmax(diff, max_error);
        }
    }
    atomicMax(error, max_error);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s max_num_iter max_toch raz\n", argv[0]);
        return 1;
    }

    const int max_num_iter = atoi(argv[1]);
    const double max_toch = atof(argv[2]);
    const int n = atoi(argv[3]);
    const int size = n * n;

    double* arr_pred;
    double* arr_new;
    double* error;

    cudaMallocManaged(&arr_pred, size * sizeof(double));
    cudaMallocManaged(&arr_new, size * sizeof(double));
    cudaMallocManaged(&error, sizeof(double));

    for (int i = 0; i < size; ++i) {
        arr_pred[i] = 0.0;
        arr_new[i] = 0.0;
    }

    arr_pred[0] = 10;
    arr_pred[n - 1] = 20;
    arr_pred[size - n] = 30;
    arr_pred[size - 1] = 20;

    const clock_t start_time = clock();

    *error = 0.0;
    int num_iter = 0;

    while (max_num_iter > num_iter && max_toch < *error) {
        jacobi_kernel<<<1, 1>>>(arr_new, arr_pred, error, n);
        cudaDeviceSynchronize();
        double max_error = *error;

        for (int i = 0; i < size; ++i) {
            arr_pred[i] = arr_new[i];
        }

        if (num_iter % 10 == 0) {
            printf("Номер итерации: %d, ошибка: %0.8lf\n", num_iter, max_error);
        }

        ++num_iter;
    }

    const clock_t end_time = clock();
    const double elapsed_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC * 1000;

    printf("Elapsed time: %0.2lf ms\n", elapsed_time);

    cudaFree(arr_pred);
    cudaFree(arr_new);
    cudaFree(error);

    return 0;
}
