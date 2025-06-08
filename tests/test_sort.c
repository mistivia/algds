#include "sort.c"

#include <assert.h>
#include <stdio.h>

int main() {
    printf("[TEST] sort\n");

    int arr[] = {2,4,5,7,4,6,7,8,8,5,4};
    int arrlen = sizeof(arr) / sizeof(int);
    Int_qsort(arr, arrlen);
    for (int i = 0; i < arrlen - 1; i++) {
        assert(arr[i] <= arr[i+1]);
    }
    Int_qsort(arr, arrlen);
    for (int i = 0; i < arrlen - 1; i++) {
        assert(arr[i] <= arr[i+1]);
    }
    printf("[PASS] sort\n");
}
