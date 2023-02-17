#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <time.h>

int N = 10000000;


int main() {

    double *arr = (double *) malloc((N + 1) * sizeof(double));
    clock_t start = clock();
    double sums = 0;
    // создание массива для видеокарты
#pragma acc kernels
    for (int i = 0; i < N; i++) {
        sums += sin(2 * M_PI * i / N);
    }
    clock_t end = clock();
    printf("time %f\n", (double)(end - start) / CLOCKS_PER_SEC);
    printf("%.50f\n", sums);


    // То же самое с float
    start = clock();
    float *arr2 = (float *) malloc((N + 1) * sizeof(float));
    float sums2 = 0;
    #pragma acc kernels
    for (int i = 0; i < N; i++) {
        sums2 += sin(2 * M_PI * i / N);
    }
    end = clock();
    printf("time %f\n", (double)(end - start) / CLOCKS_PER_SEC);

    printf("%.50f\n", sums2);
    return 0;
}
