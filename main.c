#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <time.h>

int N = 10000000;


int main() {

    double *arr = (double *) malloc((N + 1) * sizeof(double));
    // создание массива для видеокарты
#pragma acc kernels
    clock_t start = clock();
    for (int i = 0; i < N; i++) {
        arr[i] = sin(2 * M_PI * i / N);
    }
    clock_t end = clock();
    printf("time %f\n", (double)(end - start) / CLOCKS_PER_SEC);
    double sum = 0;
#pragma acc kernels
    start = clock();
    for (int i = 0; i < N; i++) {
        sum = sum + arr[i];
    }
    end = clock();
    printf("time %f\n", (double)(end - start) / CLOCKS_PER_SEC);
    printf("%.50f\n", sum);

    // То же самое с float
    float *arr2 = (float *) malloc((N + 1) * sizeof(float));
    #pragma acc kernels
    start = clock();
    for (int i = 0; i < N; i++) {
        arr2[i] = sin(2 * M_PI * i / N);
    }
    end = clock();
    printf("time %f\n", (double)(end - start) / CLOCKS_PER_SEC);
    float sum2 = 0;
    #pragma acc kernels
    start = clock();
    for (int i = 0; i < N; i++) {
        sum2 = sum2 + arr2[i];
    }
    end = clock();
    printf("time %f\n", (double)(end - start) / CLOCKS_PER_SEC);
    printf("%.50f\n", sum2);
    return 0;
}
