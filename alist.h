#ifndef ALGDS_ALIST_H_
#define ALGDS_ALIST_H_

#include <stdlib.h>
#include "type_alias.h"
#include "vec.h"

#define ALIST_DEF_AS(T, A) \
    struct A##_node { \
        T val; \
        int next; \
        int prev; \
    }; \
    typedef struct A##_node A##Node_; \
    VECTOR_DEF_AS(A##Node_, A##Vec_) \
    typedef int A##Iter; \
    typedef struct { \
        A##Vec_ vec; \
        int vhead; \
        int vtail; \
        int empty_head; \
        size_t len; \
    } A; \
    void A##_init(A *self); \
    A A##_create(); \
    void A##_free(A *self); \
    A A##_move(A *self); \
    A##Iter A##_insert_before(A *self, A##Iter iter, T val); \
    A##Iter A##_insert_after(A *self, A##Iter iter, T val); \
    void A##_remove(A *self, A##Iter iter); \
    A##Iter A##_begin(A *self); \
    A##Iter A##_last(A *self); \
    A##Iter A##_end(A *self); \
    A##Iter A##_next(A *self, A##Iter iter); \
    A##Iter A##_prev(A *self, A##Iter iter); \
    size_t A##_len(A *self); \
    A##Iter A##_push_back(A *self, T val); \
    A##Iter A##_push_front(A *self, T val); \
    void A##_pop_back(A *self); \
    void A##_pop_front(A *self); \
    T* A##_get(A *self, A##Iter iter); \
    void A##_show(A *self, FILE* fp);

ALIST_DEF_AS(Int, IntAList)
ALIST_DEF_AS(Bool, BoolAList)
ALIST_DEF_AS(Long, LongAList)
ALIST_DEF_AS(Char, CharAList)
ALIST_DEF_AS(UInt, UIntAList)
ALIST_DEF_AS(ULong, ULongAList)
ALIST_DEF_AS(Double, DoubleAList)
ALIST_DEF_AS(Float, FloatAList)
ALIST_DEF_AS(String, StringAList)
ALIST_DEF_AS(VoidPtr, VoidPtrAList)

#define ALIST_IMPL_AS(T, A) \
    void A##Node__show(A##Node_ node, FILE* fp) { \
        fprintf(fp, "{val: "); \
        T##_show(node.val, fp); \
        fprintf(fp, ", next: %d, prev: %d}", node.next, node.prev); \
    } \
    VECTOR_IMPL_AS(A##Node_, A##Vec_) \
    void A##_init(A *self) { \
        A##Vec__init(&self->vec); \
        A##Node_ vhead_node = { .next = 1, .prev = -1 }; \
        A##Node_ vtail_node = { .next = -1, .prev = 0 }; \
        A##Vec__push_back(&self->vec, vhead_node); \
        A##Vec__push_back(&self->vec, vtail_node); \
        self->vhead = 0; \
        self->vtail = 1; \
        self->empty_head = -1; \
        self->len = 0; \
    } \
    A A##_create() { \
        A self; \
        A##_init(&self); \
        return self; \
    } \
    void A##_free(A *self) { \
        A##Vec__free(&self->vec); \
    } \
    A A##_move(A *self) { \
        A dup = *self; \
        self->vec.buffer = NULL; \
        self->vec.size = 0; \
        self->vec.cap = 0; \
        self->vhead = -1; \
        self->vtail = -1; \
        self->empty_head = -1; \
        self->len = 0; \
        return dup; \
    } \
    A##Iter A##_insert_before(A *self, A##Iter iter, T val) { \
        if (iter == -1) return -1; \
        int prev_idx = A##Vec__ref(&self->vec, iter)->prev; \
        if (prev_idx == -1) return -1; \
        int new_idx; \
        if (self->empty_head != -1) { \
            new_idx = self->empty_head; \
            self->empty_head = A##Vec__ref(&self->vec, new_idx)->next; \
        } else { \
            new_idx = self->vec.size; \
            A##Node_ dummy = {0}; \
            A##Vec__push_back(&self->vec, dummy); \
        } \
        A##Node_ *node = A##Vec__ref(&self->vec, new_idx); \
        node->val = val; \
        node->next = iter; \
        node->prev = prev_idx; \
        A##Vec__ref(&self->vec, prev_idx)->next = new_idx; \
        A##Vec__ref(&self->vec, iter)->prev = new_idx; \
        self->len++; \
        return new_idx; \
    } \
    A##Iter A##_insert_after(A *self, A##Iter iter, T val) { \
        if (iter == -1) return -1; \
        int next_idx = A##Vec__ref(&self->vec, iter)->next; \
        if (next_idx == -1) return -1; \
        int new_idx; \
        if (self->empty_head != -1) { \
            new_idx = self->empty_head; \
            self->empty_head = A##Vec__ref(&self->vec, new_idx)->next; \
        } else { \
            new_idx = self->vec.size; \
            A##Node_ dummy = {0}; \
            A##Vec__push_back(&self->vec, dummy); \
        } \
        A##Node_ *node = A##Vec__ref(&self->vec, new_idx); \
        node->val = val; \
        node->prev = iter; \
        node->next = next_idx; \
        A##Vec__ref(&self->vec, next_idx)->prev = new_idx; \
        A##Vec__ref(&self->vec, iter)->next = new_idx; \
        self->len++; \
        return new_idx; \
    } \
    void A##_remove(A *self, A##Iter iter) { \
        if (iter == -1) return; \
        int p = A##Vec__ref(&self->vec, iter)->prev; \
        int n = A##Vec__ref(&self->vec, iter)->next; \
        if (p == -1 || n == -1) return; \
        A##Vec__ref(&self->vec, p)->next = n; \
        A##Vec__ref(&self->vec, n)->prev = p; \
        A##Vec__ref(&self->vec, iter)->next = self->empty_head; \
        self->empty_head = iter; \
        self->len--; \
    } \
    A##Iter A##_begin(A *self) { \
        return A##Vec__ref(&self->vec, self->vhead)->next; \
    } \
    A##Iter A##_last(A *self) { \
        int last = A##Vec__ref(&self->vec, self->vtail)->prev; \
        if (last == self->vhead) return -1; \
        return last; \
    } \
    A##Iter A##_end(A *self) { \
        return self->vtail; \
    } \
    A##Iter A##_next(A *self, A##Iter iter) { \
        if (iter == -1) return -1; \
        return A##Vec__ref(&self->vec, iter)->next; \
    } \
    A##Iter A##_prev(A *self, A##Iter iter) { \
        if (iter == -1) return -1; \
        int p = A##Vec__ref(&self->vec, iter)->prev; \
        if (p == -1 || A##Vec__ref(&self->vec, p)->prev == -1) return -1; \
        return p; \
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
        A##_remove(self, A##Vec__ref(&self->vec, self->vtail)->prev); \
    } \
    void A##_pop_front(A *self) { \
        A##_remove(self, A##Vec__ref(&self->vec, self->vhead)->next); \
    } \
    T* A##_get(A *self, A##Iter iter) { \
        if (iter == -1) return NULL; \
        return &A##Vec__ref(&self->vec, iter)->val; \
    } \
    void A##_show(A *self, FILE* fp) { \
        fprintf(fp, "["); \
        A##Iter it = A##_begin(self); \
        while (it != A##_end(self)) { \
            T##_show(*A##_get(self, it), fp); \
            it = A##_next(self, it); \
            if (it != A##_end(self)) fprintf(fp, ", "); \
        } \
        fprintf(fp, "]"); \
    }

#endif
