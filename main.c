#include <stdio.h>
#include <malloc.h>
#include <math.h>

int N=10000000;
int rat(double* arr){
#pragma acc kernels

    for (int i=0; i<N; i++){
        arr[i] = sin(2*M_PI*i/N);
    }
    return 0;
}

int main() {
    double* arr = (double*)malloc((N+1)*sizeof(double));
    // создание массива для видеокарты



    rat(arr);
    for (int i=0; i<N; i++){
        printf("%f", arr[i]);
    }
    return 0;
}