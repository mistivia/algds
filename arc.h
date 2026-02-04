#ifndef ALGDS_ARC_H_
#define ALGDS_ARC_H_

#include <stdatomic.h>
#include <stdlib.h>

#include <stdatomic.h>
#include <stdlib.h>

#define ARC(A, T) \
    typedef struct { \
        atomic_int ref_count; \
        T data; \
    } A##Ctrl; \
    \
    typedef struct { \
        T* ptr; \
        A##Ctrl* ctrl; \
    } A; \
    \
    A A##_create(T val) __attribute__((weak)); \
    A A##_copy(A *self) __attribute__((weak)); \
    void A##_destroy(A *self) __attribute__((weak)); \
    T* A##_get(A *self) __attribute__((weak)); \
    A A##_move(A *self) __attribute__((weak)); \
    \
    A A##_create(T val) { \
        A##Ctrl* ctrl = (A##Ctrl*)malloc(sizeof(A##Ctrl)); \
        atomic_init(&ctrl->ref_count, 1); \
        ctrl->data = val; \
        A arc = {0}; \
        arc.ptr = &ctrl->data; \
        arc.ctrl = ctrl; \
        return arc; \
    } \
    \
    A A##_copy(A *self) { \
        atomic_fetch_add_explicit(&self->ctrl->ref_count, 1, memory_order_relaxed); \
        A arc = {0}; \
        arc.ptr = self->ptr; \
        arc.ctrl = self->ctrl; \
        return arc; \
    } \
    \
    void A##_destroy(A *self) { \
        if (self->ctrl == NULL) return; \
        if (atomic_fetch_sub_explicit(&self->ctrl->ref_count, 1, memory_order_acq_rel) == 1) { \
            free(self->ctrl); \
        } \
        self->ptr = NULL; \
        self->ctrl = NULL; \
    } \
    \
    T* A##_get(A *self) { \
        return self->ptr; \
    } \
    \
    A A##_move(A *self) { \
        A arc = *self; \
        self->ptr = NULL; \
        self->ctrl = NULL; \
        return arc; \
    }

#endif