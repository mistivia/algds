#ifndef ALGDS_SORT_H_
#define ALGDS_SORT_H_

#include <stdlib.h>
#include <string.h>
#include "basic_types.h"

#define QSORT(T) \
    void T##_qsort(T##_t* arr, int n) __attribute__((weak)); \
    static inline void T##_qsort_swap(T##_t* arr, int lhs, int rhs) { \
        if (lhs == rhs) return; \
        T##_t buf = {0}; \
        buf = arr[lhs]; \
        arr[lhs] = arr[rhs]; \
        arr[rhs] = buf; \
    } \
    void T##_qsort(T##_t* arr, int n) { \
        if (n <= 1) return; \
        int pivot = rand() % n; \
        T##_qsort_swap(arr, 0, pivot); \
        int lp = 1, rp = n-1; \
        while (lp < rp) { \
            if (T##_cmp(&arr[lp], &arr[0]) < 0) { \
                lp++; \
                continue; \
            } \
            if (T##_cmp(&arr[rp], &arr[0]) >= 0) { \
                rp--; \
                continue; \
            } \
            T##_qsort_swap(arr, lp, rp); \
            lp++; \
            rp--; \
        } \
        if (n > 1 && T##_cmp(&arr[rp], &arr[0]) > 0) rp--; \
        T##_qsort_swap(arr, 0, rp); \
        T##_qsort(arr, rp); \
        T##_qsort(arr+rp+1, n-rp-1); \
    }

QSORT(int32);
QSORT(bool);
QSORT(int64);
QSORT(char);
QSORT(uint32);
QSORT(uint64);
QSORT(real32);
QSORT(real64);

#endif