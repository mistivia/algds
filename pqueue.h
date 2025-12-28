#ifndef PQUEUE_H_
#define PQUEUE_H_

#include "vec.h"

#define PQUEUE_DEF_AS(T, TV, A) \
    typedef struct { \
        TV vec; \
    } A; \
    void A##_init(A *self); \
    A A##_create(); \
    void A##_push(A *self, T elem); \
    void A##_pop(A *self); \
    T* A##_top(A *self); \
    A A##_move(A *self); \
    void A##_free(A *self);

#define PQUEUE_DEF(T) PQUEUE_DEF_AS(T, T##Vector, T##PQueue)

PQUEUE_DEF(Int);
PQUEUE_DEF(Bool);
PQUEUE_DEF(Long);
PQUEUE_DEF(Char);
PQUEUE_DEF(UInt);
PQUEUE_DEF(ULong);
PQUEUE_DEF(Double);
PQUEUE_DEF(Float);
PQUEUE_DEF(String);
PQUEUE_DEF(VoidPtr);

#define PQUEUE_IMPL_AS(T, TV, A) \
    static int A##_cmp(A *self, int a, int b) { \
        return T##_cmp(*TV##_ref(&self->vec, a), *TV##_ref(&self->vec, b)); \
    } \
    void A##_init(A *self) { \
         TV##_init(&self->vec); \
    } \
    A A##_create() { \
        A self; \
        A##_init(&self); \
        return self; \
    } \
    void A##_push(A *self, T elem) { \
        TV##_push_back(&self->vec, elem); \
        int i = self->vec.size - 1; \
        while (i > 0 && A##_cmp(self, i, i / 2) > 0) { \
            TV##_swap(&self->vec, i, i / 2); \
            i /= 2; \
        } \
    } \
    static void A##_heapify(A *self, int idx) { \
        int left, right, largest; \
        left = 2 * idx + 1; \
        right = 2 * idx + 2; \
        if (left < self->vec.size && A##_cmp(self, left, idx) > 0) { \
            largest = left; \
        } else { \
            largest = idx; \
        } \
        if (right < self->vec.size && A##_cmp(self, right, largest) > 0) { \
            largest = right; \
        } \
        if (largest != idx) { \
            TV##_swap(&self->vec, largest, idx); \
            A##_heapify(self, largest); \
        } \
    } \
    void A##_pop(A *self) { \
        if (self->vec.size == 0) return; \
        memcpy(TV##_ref(&self->vec, 0), TV##_ref(&self->vec, self->vec.size - 1), sizeof(T)); \
        TV##_pop(&self->vec); \
        A##_heapify(self, 0); \
    } \
    T* A##_top(A *self) { \
        if (self->vec.size == 0) return NULL; \
        return self->vec.buffer; \
    } \
    A A##_move(A *self) { \
        A dup; \
        dup.vec = TV##_move(&self->vec); \
        return dup; \
    } \
    void A##_free(A *self) { \
        TV##_free(&self->vec); \
    }

#define PQUEUE_IMPL(T) PQUEUE_IMPL_AS(T, T##Vector, T##PQueue)

#endif
