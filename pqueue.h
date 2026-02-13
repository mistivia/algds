#ifndef ALGDS_PQUEUE_H_
#define ALGDS_PQUEUE_H_

#include "vec.h"

#define PQUEUE(A, T) \
    VECTOR(A##_vec_, T); \
    typedef struct { \
        A##_vec__t vec; \
    } A##_t; \
    void A##_init(A##_t *self) __attribute__((weak)); \
    A##_t A##_create() __attribute__((weak)); \
    void A##_push(A##_t *self, T##_t val) __attribute__((weak)); \
    void A##_pop(A##_t *self) __attribute__((weak)); \
    T##_t* A##_top(A##_t *self) __attribute__((weak)); \
    A##_t A##_move(A##_t *self) __attribute__((weak)); \
    void A##_destroy(A##_t *self) __attribute__((weak)); \
    A##_t A##_copy(A##_t *self) __attribute__((weak)); \
    \
    static inline int A##_cmp(A##_t *self, int a, int b) { \
        return T##_cmp(A##_vec__ref(&self->vec, a), A##_vec__ref(&self->vec, b)); \
    } \
    void A##_init(A##_t *self) { \
        self->vec = A##_vec__create(); \
    } \
    A##_t A##_create() { \
        A##_t self; \
        A##_init(&self); \
        return self; \
    } \
    void A##_push(A##_t *self, T##_t val) { \
        A##_vec__push_back(&self->vec, val); \
        int i = self->vec.size - 1; \
        while (i > 0 && A##_cmp(self, i, i / 2) > 0) { \
            A##_vec__swap(&self->vec, i, i / 2); \
            i /= 2; \
        } \
    } \
    static inline void A##_heapify(A##_t *self, int idx) { \
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
            A##_vec__swap(&self->vec, largest, idx); \
            A##_heapify(self, largest); \
        } \
    } \
    void A##_pop(A##_t *self) { \
        if (self->vec.size == 0) return; \
        memcpy(A##_vec__ref(&self->vec, 0), A##_vec__ref(&self->vec, self->vec.size - 1), sizeof(T##_t)); \
        A##_vec__pop(&self->vec); \
        A##_heapify(self, 0); \
    } \
    T##_t* A##_top(A##_t *self) { \
        if (self->vec.size == 0) return NULL; \
        return self->vec.buffer; \
    } \
    A##_t A##_move(A##_t *self) { \
        A##_t dup; \
        dup.vec = A##_vec__move(&self->vec); \
        return dup; \
    } \
    void A##_destroy(A##_t *self) { \
        A##_vec__destroy(&self->vec); \
    } \
    A##_t A##_copy(A##_t *self) { \
        A##_t ret = {0}; \
        ret.vec = A##_vec__copy(&self->vec); \
        return ret; \
    }

#endif
