#ifndef ALGDS_LIST_H_
#define ALGDS_LIST_H_

#include <stdlib.h>
#include <stdbool.h>

#define LIST(A, T) \
    typedef struct A##_node_ { \
        T##_t val; \
        struct A##_node_ *prev; \
        struct A##_node_ *next; \
    } A##_node_; \
    typedef A##_node_ *A##_iter_t; \
    typedef struct { \
        A##_node_ *vhead; \
        A##_node_ *vtail; \
        size_t len; \
    } A##_t; \
    \
    void A##_init(A##_t *self) __attribute__((weak)); \
    A##_t A##_create() __attribute__((weak)); \
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
    \
    void A##_init(A##_t *self) { \
        self->vhead = malloc(sizeof(A##_node_)); \
        self->vtail = malloc(sizeof(A##_node_)); \
        self->vhead->next = self->vtail; \
        self->vhead->prev = NULL; \
        self->vtail->next = NULL; \
        self->vtail->prev = self->vhead; \
        self->len = 0; \
    } \
    A##_t A##_create() { \
        A##_t self; \
        A##_init(&self); \
        return self; \
    } \
    void A##_destroy(A##_t *self) { \
        A##_iter_t cur = self->vhead; \
        while (cur != NULL) { \
            A##_iter_t next = cur->next; \
            if (cur != self->vhead && cur != self->vtail) { \
                T##_destroy(&cur->val); \
            } \
            free(cur); \
            cur = next; \
        } \
        self->vhead = NULL; \
        self->vtail = NULL; \
        self->len = 0; \
    } \
    A##_t A##_move(A##_t *self) { \
        A##_t dup; \
        dup.vhead = self->vhead; \
        dup.vtail = self->vtail; \
        dup.len = self->len; \
        self->vhead = NULL; \
        self->vtail = NULL; \
        self->len = 0; \
        return dup; \
    } \
    A##_iter_t A##_insert_before(A##_t *self, A##_iter_t iter, T##_t val) { \
        if (iter->prev == NULL) return NULL; \
        A##_iter_t newnode = malloc(sizeof(A##_node_)); \
        newnode->prev = iter->prev; \
        newnode->next = iter; \
        newnode->val = val; \
        iter->prev->next = newnode; \
        iter->prev = newnode; \
        self->len++; \
        return newnode; \
    } \
    A##_iter_t A##_insert_after(A##_t *self, A##_iter_t iter, T##_t val) { \
        if (iter->next == NULL) return NULL; \
        A##_iter_t newnode = malloc(sizeof(A##_node_)); \
        newnode->next = iter->next; \
        newnode->prev= iter; \
        newnode->val = val; \
        iter->next->prev= newnode; \
        iter->next = newnode; \
        self->len++; \
        return newnode; \
    } \
    void A##_remove(A##_t *self, A##_iter_t iter) { \
        if (iter->prev == NULL || iter->next == NULL) return; \
        iter->prev->next = iter->next; \
        iter->next->prev = iter->prev; \
        T##_destroy(&iter->val); \
        free(iter); \
        self->len--; \
    } \
    A##_iter_t A##_begin(A##_t *self) { \
        return self->vhead->next; \
    } \
    A##_iter_t A##_last(A##_t *self) { \
        if (self->vtail->prev == self->vhead) return NULL; \
        return self->vtail->prev; \
    } \
    A##_iter_t A##_end(A##_t *self) { \
        return self->vtail; \
    } \
    A##_iter_t A##_next(A##_t *self, A##_iter_t iter) { \
        (void)self; \
        if (iter == NULL) return NULL; \
        return iter->next; \
    } \
    A##_iter_t A##_prev(A##_iter_t iter) { \
        if (iter == NULL) return NULL; \
        if (iter->prev == NULL) return NULL; \
        if (iter->prev->prev == NULL) return NULL; \
        return iter->prev; \
    } \
    size_t A##_len(A##_t *self) { \
        return self->len; \
    } \
    A##_iter_t A##_push_back(A##_t *self, T##_t val) { \
        return A##_insert_before(self, self->vtail, val); \
    } \
    A##_iter_t A##_push_front(A##_t *self, T##_t val) { \
        return A##_insert_after(self, self->vhead, val); \
    } \
    void A##_pop_back(A##_t *self) { \
        A##_remove(self, self->vtail->prev); \
    } \
    void A##_pop_front(A##_t *self) { \
        A##_remove(self, self->vhead->next); \
    }

#endif
