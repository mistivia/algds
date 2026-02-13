#ifndef ALGDS_ARC_H_
#define ALGDS_ARC_H_

#include <stdatomic.h>
#include <stdlib.h>

#define ARC(A, T) \
    typedef struct { \
        atomic_int ref_count; \
        T##_t data; \
    } A##_ctrl_; \
    \
    typedef struct { \
        T##_t* ptr; \
        A##_ctrl_* ctrl; \
    } A##_t; \
    \
    A##_t A##_create(T##_t val) __attribute__((weak)); \
    A##_t A##_copy(A##_t *self) __attribute__((weak)); \
    void A##_destroy(A##_t *self) __attribute__((weak)); \
    T##_t* A##_get(A##_t *self) __attribute__((weak)); \
    A##_t A##_move(A##_t *self) __attribute__((weak)); \
    \
    A##_t A##_create(T##_t val) { \
        A##_ctrl_* ctrl = (A##_ctrl_*)malloc(sizeof(A##_ctrl_)); \
        atomic_init(&ctrl->ref_count, 1); \
        ctrl->data = val; \
        A##_t arc = {0}; \
        arc.ptr = &ctrl->data; \
        arc.ctrl = ctrl; \
        return arc; \
    } \
    \
    A##_t A##_copy(A##_t *self) { \
        atomic_fetch_add_explicit(&self->ctrl->ref_count, 1, memory_order_relaxed); \
        A##_t arc = {0}; \
        arc.ptr = self->ptr; \
        arc.ctrl = self->ctrl; \
        return arc; \
    } \
    \
    void A##_destroy(A##_t *self) { \
        if (self->ctrl == NULL) return; \
        if (atomic_fetch_sub_explicit(&self->ctrl->ref_count, 1, memory_order_acq_rel) == 1) { \
            free(self->ctrl); \
        } \
        self->ptr = NULL; \
        self->ctrl = NULL; \
    } \
    \
    T##_t* A##_get(A##_t *self) { \
        return self->ptr; \
    } \
    \
    A##_t A##_move(A##_t *self) { \
        A##_t arc = *self; \
        self->ptr = NULL; \
        self->ctrl = NULL; \
        return arc; \
    }

#endif
