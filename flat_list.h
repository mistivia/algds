#ifndef ALGDS_FLAT_LIST_H_
#define ALGDS_FLAT_LIST_H_

#include <stdlib.h>
#include <stdbool.h>
#include "vec.h"

#define FLAT_LIST(A, T) \
    typedef struct A##_node_ { \
        T##_t val; \
        int prev; \
        int next; \
        bool initialized; \
    } A##_node__t; \
    \
    /* Define destroy and copy functions for the node type */ \
    static inline void A##_node__destroy(A##_node__t *self) { \
        if (self->initialized) { \
            T##_destroy(&self->val); \
            self->initialized = false; \
        } \
    } \
    \
    static inline A##_node__t A##_node__copy(A##_node__t *self) { \
        A##_node__t ret; \
        ret.val = T##_copy(&self->val); \
        ret.prev = self->prev; \
        ret.next = self->next; \
        ret.initialized = self->initialized; \
        return ret; \
    } \
    \
    /* Define vector for nodes */ \
    VECTOR(A##_node_vec, A##_node_); \
    \
    typedef int A##_iter_t; \
    typedef struct { \
        A##_node_vec_t nodes; \
        size_t len; \
        int free_head; \
        int vhead; \
        int vtail; \
    } A##_t; \
    \
    void A##_init(A##_t *self) __attribute__((weak)); \
    A##_t A##_create(void) __attribute__((weak)); \
    void A##_destroy(A##_t *self) __attribute__((weak)); \
    A##_t A##_move(A##_t *self) __attribute__((weak)); \
    A##_iter_t A##_insert_before(A##_t *self, A##_iter_t iter, T##_t val) __attribute__((weak)); \
    A##_iter_t A##_insert_after(A##_t *self, A##_iter_t iter, T##_t val) __attribute__((weak)); \
    void A##_remove(A##_t *self, A##_iter_t iter) __attribute__((weak)); \
    A##_iter_t A##_begin(A##_t *self) __attribute__((weak)); \
    A##_iter_t A##_last(A##_t *self) __attribute__((weak)); \
    A##_iter_t A##_end(A##_t *self) __attribute__((weak)); \
    A##_iter_t A##_next(A##_t *self, A##_iter_t iter) __attribute__((weak)); \
    A##_iter_t A##_prev(A##_iter_t iter) __attribute__((weak)); \
    size_t A##_len(A##_t *self) __attribute__((weak)); \
    A##_iter_t A##_push_back(A##_t *self, T##_t val) __attribute__((weak)); \
    A##_iter_t A##_push_front(A##_t *self, T##_t val) __attribute__((weak)); \
    void A##_pop_back(A##_t *self) __attribute__((weak)); \
    void A##_pop_front(A##_t *self) __attribute__((weak)); \
    T##_t *A##_get(A##_t *self, A##_iter_t iter) __attribute__((weak)); \
    T##_t *A##_front(A##_t *self) __attribute__((weak)); \
    T##_t *A##_back(A##_t *self) __attribute__((weak)); \
    \
    static inline int A##_alloc_node(A##_t *self) { \
        if (self->free_head == -1) { \
            /* Need to expand - add new nodes to free list */ \
            int old_size = self->nodes.size; \
            int new_size = old_size + 16; \
            for (int i = old_size; i < new_size; i++) { \
                A##_node__t free_node = { .prev = -1, .next = i + 1, .initialized = false }; \
                A##_node_vec_push_back(&self->nodes, free_node); \
            } \
            A##_node_vec_ref(&self->nodes, new_size - 1)->next = self->free_head; \
            self->free_head = old_size; \
        } \
        int idx = self->free_head; \
        A##_node__t *node = A##_node_vec_ref(&self->nodes, idx); \
        self->free_head = node->next; \
        node->prev = -1; \
        node->next = -1; \
        node->initialized = false; \
        return idx; \
    } \
    \
    static inline void A##_free_node(A##_t *self, int idx) { \
        if (idx < 2) return; /* Don't free sentinel nodes */ \
        A##_node__t *node = A##_node_vec_ref(&self->nodes, idx); \
        node->next = self->free_head; \
        node->prev = -1; \
        node->initialized = false; \
        self->free_head = idx; \
    } \
    \
    void A##_init(A##_t *self) { \
        self->nodes = A##_node_vec_create(); \
        self->len = 0; \
        self->free_head = 2; \
        self->vhead = 0; \
        self->vtail = 1; \
        /* Add sentinel nodes */ \
        A##_node__t head_node = { .prev = -1, .next = 1, .initialized = false }; \
        A##_node_vec_push_back(&self->nodes, head_node); \
        A##_node__t tail_node = { .prev = 0, .next = -1, .initialized = false }; \
        A##_node_vec_push_back(&self->nodes, tail_node); \
        /* Initialize free list with some initial capacity */ \
        for (int i = 2; i < 16; i++) { \
            A##_node__t free_node = { .prev = -1, .next = i + 1, .initialized = false }; \
            A##_node_vec_push_back(&self->nodes, free_node); \
        } \
        if (self->nodes.size > 2) { \
            A##_node_vec_ref(&self->nodes, self->nodes.size - 1)->next = -1; \
        } \
    } \
    \
    A##_t A##_create(void) { \
        A##_t self; \
        A##_init(&self); \
        return self; \
    } \
    \
    void A##_destroy(A##_t *self) { \
        if (self->nodes.buffer == NULL) return; \
        A##_node_vec_destroy(&self->nodes); \
        self->len = 0; \
        self->free_head = -1; \
        self->vhead = -1; \
        self->vtail = -1; \
    } \
    \
    A##_t A##_move(A##_t *self) { \
        A##_t dup; \
        dup.nodes = A##_node_vec_move(&self->nodes); \
        dup.len = self->len; \
        dup.free_head = self->free_head; \
        dup.vhead = self->vhead; \
        dup.vtail = self->vtail; \
        self->len = 0; \
        self->free_head = -1; \
        self->vhead = -1; \
        self->vtail = -1; \
        return dup; \
    } \
    \
    A##_iter_t A##_insert_before(A##_t *self, A##_iter_t iter, T##_t val) { \
        if (iter < 0 || iter >= self->nodes.size) return -1; \
        A##_node__t *iter_node = A##_node_vec_ref(&self->nodes, iter); \
        if (iter_node->prev < 0) return -1; \
        int new_idx = A##_alloc_node(self); \
        iter_node = A##_node_vec_ref(&self->nodes, iter); \
        if (new_idx < 0) return -1; \
        A##_node__t *new_node = A##_node_vec_ref(&self->nodes, new_idx); \
        new_node->val = val; \
        new_node->initialized = true; \
        new_node->prev = iter_node->prev; \
        new_node->next = iter; \
        A##_node_vec_ref(&self->nodes, iter_node->prev)->next = new_idx; \
        iter_node->prev = new_idx; \
        self->len++; \
        return new_idx; \
    } \
    \
    A##_iter_t A##_insert_after(A##_t *self, A##_iter_t iter, T##_t val) { \
        if (iter < 0 || iter >= self->nodes.size) return -1; \
        A##_node__t *iter_node = A##_node_vec_ref(&self->nodes, iter); \
        if (iter_node->next < 0) return -1; \
        int new_idx = A##_alloc_node(self); \
        iter_node = A##_node_vec_ref(&self->nodes, iter); \
        if (new_idx < 0) return -1; \
        A##_node__t *new_node = A##_node_vec_ref(&self->nodes, new_idx); \
        new_node->val = val; \
        new_node->initialized = true; \
        new_node->next = iter_node->next; \
        new_node->prev = iter; \
        A##_node_vec_ref(&self->nodes, iter_node->next)->prev = new_idx; \
        iter_node->next = new_idx; \
        self->len++; \
        return new_idx; \
    } \
    \
    void A##_remove(A##_t *self, A##_iter_t iter) { \
        if (iter < 2 || iter >= self->nodes.size) return; \
        A##_node__t *node = A##_node_vec_ref(&self->nodes, iter); \
        int prev_idx = node->prev; \
        int next_idx = node->next; \
        if (prev_idx < 0 || next_idx < 0) return; \
        A##_node_vec_ref(&self->nodes, prev_idx)->next = next_idx; \
        A##_node_vec_ref(&self->nodes, next_idx)->prev = prev_idx; \
        T##_destroy(&node->val); \
        A##_free_node(self, iter); \
        self->len--; \
    } \
    \
    A##_iter_t A##_begin(A##_t *self) { \
        return A##_node_vec_ref(&self->nodes, self->vhead)->next; \
    } \
    \
    A##_iter_t A##_last(A##_t *self) { \
        int prev = A##_node_vec_ref(&self->nodes, self->vtail)->prev; \
        if (prev == self->vhead) return -1; \
        return prev; \
    } \
    \
    A##_iter_t A##_end(A##_t *self) { \
        (void)self; \
        return self->vtail; \
    } \
    \
    A##_iter_t A##_next(A##_t *self, A##_iter_t iter) { \
        (void)self; \
        if (iter < 0 || iter >= self->nodes.size) return -1; \
        return A##_node_vec_ref(&self->nodes, iter)->next; \
    } \
    \
    A##_iter_t A##_prev(A##_iter_t iter) { \
        if (iter < 0) return -1; \
        return iter - 1; \
    } \
    \
    size_t A##_len(A##_t *self) { \
        return self->len; \
    } \
    \
    A##_iter_t A##_push_back(A##_t *self, T##_t val) { \
        return A##_insert_before(self, self->vtail, val); \
    } \
    \
    A##_iter_t A##_push_front(A##_t *self, T##_t val) { \
        return A##_insert_after(self, self->vhead, val); \
    } \
    \
    void A##_pop_back(A##_t *self) { \
        int last = A##_last(self); \
        if (last >= 0) A##_remove(self, last); \
    } \
    \
    void A##_pop_front(A##_t *self) { \
        int first = A##_begin(self); \
        if (first >= 0) A##_remove(self, first); \
    } \
    \
    T##_t *A##_get(A##_t *self, A##_iter_t iter) { \
        if (iter <= 1 || iter >= self->nodes.size) return NULL; \
        return &A##_node_vec_ref(&self->nodes, iter)->val; \
    } \
    \
    T##_t *A##_front(A##_t *self) { \
        int first = A##_begin(self); \
        if (first <= 1) return NULL; \
        return &A##_node_vec_ref(&self->nodes, first)->val; \
    } \
    \
    T##_t *A##_back(A##_t *self) { \
        int last = A##_last(self); \
        if (last <= 1) return NULL; \
        return &A##_node_vec_ref(&self->nodes, last)->val; \
    }

#endif
