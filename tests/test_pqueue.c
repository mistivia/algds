#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pqueue.h"
#include "basic_types.h"

typedef int32_t min_int_t;

int min_int_cmp(int32_t *lhs, int32_t *rhs) {
    return -int32_cmp(lhs, rhs);
}
void min_int_destroy(min_int_t *i) {}

min_int_t min_int_copy(min_int_t *self) { return *self; }

PQUEUE(int_pqueue, int32)
PQUEUE(min_int_pqueue, min_int)

int main() {
    printf("[TEST] pqueue\n");

    int_pqueue_t pq;
    int_pqueue_init(&pq);
    int elems[10] = {1, 3, 2, 4, 6, 5, 9, 7, 8, 10};
    for (int i = 0; i < 10; i++) {
        int32_t e = elems[i];
        int_pqueue_push(&pq, e);
    }
    for (int i = 10; i >= 1; i--) {
        int32_t *top = int_pqueue_top(&pq);
        assert(i == *top);
        int_pqueue_pop(&pq);
    }
    assert(int_pqueue_top(&pq) == NULL);


    min_int_pqueue_t minpq;
    min_int_pqueue_init(&minpq);
    for (int i = 0; i < 10; i++) {
        int32_t e = elems[i];
        min_int_pqueue_push(&minpq, e);
    }
    for (int i = 1; i <= 10; i++) {
        int32_t *top = min_int_pqueue_top(&minpq);
        assert(i == *top);
        min_int_pqueue_pop(&minpq);
    }
    assert(min_int_pqueue_top(&minpq) == NULL);
    min_int_pqueue_destroy(&minpq);

    int elems2[10] = {-10, -8, -7, -9, -5, -6, -4, -2, -3, -1};
    int expected[10] = {-10, -8, -7, -7, -5, -5, -4, -2, -2, -1};
    for (int i = 0; i < 10; i++) {
        int32_t e = elems2[i];
        int_pqueue_push(&pq, e);
        int32_t *top = int_pqueue_top(&pq);
        assert(*top == expected[i]);
    }
    int_pqueue_destroy(&pq);
    printf("[PASS] pqueue\n");
    return 0;
}
