#ifndef PQUEUE_H_
#define PQUEUE_H_

#include "vec.h"

#define PQUEUE_DEF_AS(T, A) \
    VECTOR_DEF_AS(T, A##Vec_); \
    typedef struct { \
        A##Vec_ vec; \
    } A; \
    void A##_init(A *self); \
    A A##_create(); \
    void A##_push(A *self, T elem); \
    void A##_pop(A *self); \
    T* A##_top(A *self); \
    A A##_move(A *self); \
    void A##_free(A *self);

#define PQUEUE_DEF(T) PQUEUE_DEF_AS(T, T##PQueue)

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

#define PQUEUE_IMPL_AS(T, A) \
    VECTOR_IMPL_AS(T, A##Vec_); \
    static int A##_cmp(A *self, int a, int b) { \
        return T##_cmp(*A##Vec__ref(&self->vec, a), *A##Vec__ref(&self->vec, b)); \
    } \
    void A##_init(A *self) { \
         A##Vec__init(&self->vec); \
    } \
    A A##_create() { \
        A self; \
        A##_init(&self); \
        return self; \
    } \
    void A##_push(A *self, T elem) { \
        A##Vec__push_back(&self->vec, elem); \
        int i = self->vec.size - 1; \
        while (i > 0 && A##_cmp(self, i, i / 2) > 0) { \
            A##Vec__swap(&self->vec, i, i / 2); \
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
            A##Vec__swap(&self->vec, largest, idx); \
            A##_heapify(self, largest); \
        } \
    } \
    void A##_pop(A *self) { \
        if (self->vec.size == 0) return; \
        memcpy(A##Vec__ref(&self->vec, 0), A##Vec__ref(&self->vec, self->vec.size - 1), sizeof(T)); \
        A##Vec__pop(&self->vec); \
        A##_heapify(self, 0); \
    } \
    T* A##_top(A *self) { \
        if (self->vec.size == 0) return NULL; \
        return self->vec.buffer; \
    } \
    A A##_move(A *self) { \
        A dup; \
        dup.vec = A##Vec__move(&self->vec); \
        return dup; \
    } \
    void A##_free(A *self) { \
        A##Vec__free(&self->vec); \
    }

#define PQUEUE_IMPL(T) PQUEUE_IMPL_AS(T, T##PQueue)

#endif
