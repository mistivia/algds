#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pqueue.h"
#include "basic_traits.h"

typedef Int MinInt;

int MinInt_cmp(Int *lhs, Int *rhs) {
    return -Int_cmp(lhs, rhs);
}

VECTOR_DEF(MinInt);
VECTOR_IMPL(MinInt);

PQUEUE_DEF(MinInt);
PQUEUE_IMPL(MinInt);

int main() {
    printf("[TEST] pqueue\n");

    IntPQueue pq;
    IntPQueue_init(&pq);
    int elems[10] = {1, 3, 2, 4, 6, 5, 9, 7, 8, 10};
    for (int i = 0; i < 10; i++) {
        int e = elems[i];
        IntPQueue_push(&pq, &e);
    }
    for (int i = 10; i >= 1; i--) {
        int *top = IntPQueue_top(&pq);
        assert(i == *top);
        IntPQueue_pop(&pq);
    }
    assert(IntPQueue_top(&pq) == NULL);


    MinIntPQueue minpq;
    MinIntPQueue_init(&minpq);
    for (int i = 0; i < 10; i++) {
        int e = elems[i];
        MinIntPQueue_push(&minpq, &e);
    }
    for (int i = 1; i <= 10; i++) {
        int *top = MinIntPQueue_top(&minpq);
        assert(i == *top);
        MinIntPQueue_pop(&minpq);
    }
    assert(MinIntPQueue_top(&minpq) == NULL);
    MinIntVector_free(&minpq.vec);

    int elems2[10] = {-10, -8, -7, -9, -5, -6, -4, -2, -3, -1};
    int expected[10] = {-10, -8, -7, -7, -5, -5, -4, -2, -2, -1};
    for (int i = 0; i < 10; i++) {
        int e = elems2[i];
        IntPQueue_push(&pq, &e);
        int *top = IntPQueue_top(&pq);
        assert(*top == expected[i]);
    }
    IntVector_free(&pq.vec);
    printf("[PASS] pqueue\n");
    return 0;
}
