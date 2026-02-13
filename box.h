#ifndef ALGDS_BOX_H_
#define ALGDS_BOX_H_

#include <stdlib.h>

#define BOX(A, T) \
    typedef struct { \
        T##_t* ptr; \
    } A##_t; \
    \
    A##_t A##_create(T##_t val) __attribute__((weak)); \
    void A##_destroy(A##_t *self) __attribute__((weak)); \
    T##_t* A##_get(A##_t *self) __attribute__((weak)); \
    A##_t A##_move(A##_t *self) __attribute__((weak)); \
    A##_t A##_copy(A##_t *self); /* = delete */ \
    \
    A##_t A##_create(T##_t val) { \
        T##_t* ptr = (T##_t*)malloc(sizeof(T##_t)); \
        *ptr = val; \
        A##_t box = {0}; \
        box.ptr = ptr; \
        return box; \
    } \
    \
    void A##_destroy(A##_t *self) { \
        if (self->ptr == NULL) return; \
        free(self->ptr); \
        self->ptr = NULL; \
    } \
    \
    T##_t* A##_get(A##_t *self) { \
        return self->ptr; \
    } \
    \
    A##_t A##_move(A##_t *self) { \
        A##_t box = *self; \
        self->ptr = NULL; \
        return box; \
    }

#endif
