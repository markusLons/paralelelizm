#include <stdio.h>
#include <malloc.h>
#include <math.h>

int N = 10000000;


int main() {

    double *arr = (double *) malloc((N + 1) * sizeof(double));
    // создание массива для видеокарты
#pragma acc kernels
    for (int i = 0; i < N; i++) {
        arr[i] = sin(2 * M_PI * i / N);
    }
    double sum = 0;
#pragma acc kernels
    for (int i = 0; i < N; i++) {
        sum = sum + arr[i];
    }
    printf("%.50f", sum);
    return 0;
}
