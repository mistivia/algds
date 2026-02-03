#ifndef ALGDS_BOX_H_
#define ALGDS_BOX_H_

#include <stdlib.h>

#define BOX(A, T) \
    typedef struct { \
        T* ptr; \
    } A; \
    \
    A A##_create(T val) __attribute__((weak)); \
    void A##_destroy(A *self) __attribute__((weak)); \
    T* A##_get(A *self) __attribute__((weak)); \
    A A##_move(A *self) __attribute__((weak)); \
    A A##_copy(A *self); \
    \
    A A##_create(T val) { \
        T* ptr = (T*)malloc(sizeof(T)); \
        *ptr = val; \
        A box; \
        box.ptr = ptr; \
        return box; \
    } \
    \
    void A##_destroy(A *self) { \
        if (self->ptr == NULL) return; \
        free(self->ptr); \
        self->ptr = NULL; \
    } \
    \
    T* A##_get(A *self) { \
        return self->ptr; \
    } \
    \
    A A##_move(A *self) { \
        A box = *self; \
        self->ptr = NULL; \
        return box; \
    }

#endif