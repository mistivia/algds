#ifndef ALGDS_LIST_H_
#define ALGDS_LIST_H_

#include <stdlib.h>
#include <stdbool.h>

#define LIST(A, T) \
    typedef struct A##Node_ { \
        T val; \
        struct A##Node_ *prev; \
        struct A##Node_ *next; \
    } A##Node_; \
    typedef A##Node_ *A##Iter; \
    typedef struct { \
        A##Node_ *vhead; \
        A##Node_ *vtail; \
        size_t len; \
    } A; \
    \
    void A##_init(A *self) __attribute__((weak)); \
    A A##_create() __attribute__((weak)); \
    void A##_destroy(A *self) __attribute__((weak)); \
    A A##_move(A *self) __attribute__((weak)); \
    A##Iter A##_insert_before(A *self, A##Iter iter, T val) __attribute__((weak)); \
    A##Iter A##_insert_after(A *self, A##Iter iter, T val) __attribute__((weak)); \
    void A##_remove(A *self, A##Iter iter) __attribute__((weak)); \
    A##Iter A##_begin(A *self) __attribute__((weak)); \
    A##Iter A##_last(A *self) __attribute__((weak)); \
    A##Iter A##_end(A *self) __attribute__((weak)); \
    A##Iter A##_next(A##Iter iter) __attribute__((weak)); \
    A##Iter A##_prev(A##Iter iter) __attribute__((weak)); \
    size_t A##_len(A *self) __attribute__((weak)); \
    A##Iter A##_push_back(A *self, T val) __attribute__((weak)); \
    A##Iter A##_push_front(A *self, T val) __attribute__((weak)); \
    void A##_pop_back(A *self) __attribute__((weak)); \
    void A##_pop_front(A *self) __attribute__((weak)); \
    \
    void A##_init(A *self) { \
        self->vhead = malloc(sizeof(A##Node_)); \
        self->vtail = malloc(sizeof(A##Node_)); \
        self->vhead->next = self->vtail; \
        self->vhead->prev = NULL; \
        self->vtail->next = NULL; \
        self->vtail->prev = self->vhead; \
        self->len = 0; \
    } \
    A A##_create() { \
        A self; \
        A##_init(&self); \
        return self; \
    } \
    void A##_destroy(A *self) { \
        A##Iter cur = self->vhead; \
        while (cur != NULL) { \
            A##Iter next = cur->next; \
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
    A A##_move(A *self) { \
        A dup; \
        dup.vhead = self->vhead; \
        dup.vtail = self->vtail; \
        dup.len = self->len; \
        self->vhead = NULL; \
        self->vtail = NULL; \
        self->len = 0; \
        return dup; \
    } \
    A##Iter A##_insert_before(A *self, A##Iter iter, T val) { \
        if (iter->prev == NULL) return NULL; \
        A##Iter newnode = malloc(sizeof(A##Node_)); \
        newnode->prev = iter->prev; \
        newnode->next = iter; \
        newnode->val = val; \
        iter->prev->next = newnode; \
        iter->prev = newnode; \
        self->len++; \
        return newnode; \
    } \
    A##Iter A##_insert_after(A *self, A##Iter iter, T val) { \
        if (iter->next == NULL) return NULL; \
        A##Iter newnode = malloc(sizeof(A##Node_)); \
        newnode->next = iter->next; \
        newnode->prev= iter; \
        newnode->val = val; \
        iter->next->prev= newnode; \
        iter->next = newnode; \
        self->len++; \
        return newnode; \
    } \
    void A##_remove(A *self, A##Iter iter) { \
        if (iter->prev == NULL || iter->next == NULL) return; \
        iter->prev->next = iter->next; \
        iter->next->prev = iter->prev; \
        T##_destroy(&iter->val); \
        free(iter); \
        self->len--; \
    } \
    A##Iter A##_begin(A *self) { \
        return self->vhead->next; \
    } \
    A##Iter A##_last(A *self) { \
        if (self->vtail->prev == self->vhead) return NULL; \
        return self->vtail->prev; \
    } \
    A##Iter A##_end(A *self) { \
        return self->vtail; \
    } \
    A##Iter A##_next(A##Iter iter) { \
        if (iter == NULL) return NULL; \
        return iter->next; \
    } \
    A##Iter A##_prev(A##Iter iter) { \
        if (iter == NULL) return NULL; \
        if (iter->prev == NULL) return NULL; \
        if (iter->prev->prev == NULL) return NULL; \
        return iter->prev; \
    } \
    size_t A##_len(A *self) { \
        return self->len; \
    } \
    A##Iter A##_push_back(A *self, T val) { \
        return A##_insert_before(self, self->vtail, val); \
    } \
    A##Iter A##_push_front(A *self, T val) { \
        return A##_insert_after(self, self->vhead, val); \
    } \
    void A##_pop_back(A *self) { \
        A##_remove(self, self->vtail->prev); \
    } \
    void A##_pop_front(A *self) { \
        A##_remove(self, self->vhead->next); \
    }

#endif