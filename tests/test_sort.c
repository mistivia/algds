#include "sort.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int compareInts(const void *a, const void *b) {
    int arg1 = *(const int *)a;
    int arg2 = *(const int *)b;
    return (arg1 > arg2) - (arg1 < arg2);
}

void rand_test(int n) {
    int *arr1 = malloc(sizeof(int) * n);
    int *arr2 = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        int val = rand();
        arr1[i] = val;
        arr2[i] = val;
    }
    qsort(arr1, n, sizeof(int), compareInts);
    int32_qsort(arr2, n);
    for (int i = 0; i < n; i++) {
        assert(arr1[i] == arr2[i]);
    }
    free(arr1);
    free(arr2);
}

int main() {
    srand(time(NULL));
    printf("[TEST] sort\n");

    int arr[] = {2,4,5,7,4,6,7,8,8,5,4};
    int arrlen = sizeof(arr) / sizeof(int);
    int32_qsort(arr, arrlen);
    for (int i = 0; i < arrlen - 1; i++) {
        assert(arr[i] <= arr[i+1]);
    }
    int32_qsort(arr, arrlen);
    for (int i = 0; i < arrlen - 1; i++) {
        assert(arr[i] <= arr[i+1]);
    }
    for (int i = 0; i < 10; i++) {
        rand_test(rand() % 10000);
    }
    printf("[PASS] sort\n");
}
