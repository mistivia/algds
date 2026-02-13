/*
 * Copyright 2002 Niels Provos <provos@citi.umich.edu>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef RB_TREE_H_
#define RB_TREE_H_

#include <stdlib.h>

#define TREE_MAP(A, K, V) \
    typedef struct { \
        RBNode rbnode; \
        K##_t key; \
        V##_t value; \
    } A##_node_; \
    typedef A##_node_ *A##_iter_t; \
    typedef struct { \
        RBTree tree; \
    } A##_t; \
    \
    void A##_init(A##_t *self) __attribute__((weak)); \
    A##_t A##_create() __attribute__((weak)); \
    A##_iter_t A##_insert(A##_t *self, K##_t key, V##_t value) __attribute__((weak)); \
    A##_iter_t A##_find(A##_t *self, K##_t key) __attribute__((weak)); \
    V##_t* A##_get(A##_t *self, K##_t key) __attribute__((weak)); \
    A##_iter_t A##_next(A##_t *self, A##_iter_t iter) __attribute__((weak)); \
    A##_iter_t A##_min(A##_t *self) __attribute__((weak)); \
    A##_iter_t A##_max(A##_t *self) __attribute__((weak)); \
    void A##_remove(A##_t *self, A##_iter_t iter) __attribute__((weak)); \
    A##_iter_t A##_left(A##_iter_t iter) __attribute__((weak)); \
    A##_iter_t A##_right(A##_iter_t iter) __attribute__((weak)); \
    A##_iter_t A##_parent(A##_iter_t iter) __attribute__((weak)); \
    void A##_destroy(A##_t *self) __attribute__((weak)); \
    A##_t A##_copy(A##_t *self) __attribute__((weak)); \
    A##_t A##_move(A##_t *self) __attribute__((weak)); \
    \
    \
    static inline int A##_cmp(void *vlhs, void *vrhs) { \
        K##_t *lhs = vlhs, *rhs = vrhs; \
        return K##_cmp(lhs, rhs); \
    } \
    void A##_init(A##_t *self) { \
        self->tree.rbh_root = NULL; \
        self->tree.cmp = A##_cmp; \
        self->tree.augment = NULL; \
    } \
    A##_t A##_create() { \
        A##_t self = {0}; \
        A##_init(&self); \
        return self; \
    } \
    A##_iter_t A##_insert(A##_t *self, K##_t key, V##_t value) { \
        A##_node_ *newnode = malloc(sizeof(A##_node_)); \
        newnode->key = key; \
        newnode->value = value; \
        return rb_tree_insert(&self->tree, newnode); \
    } \
    A##_iter_t A##_find(A##_t *self, K##_t key) { \
        return rb_tree_find(&self->tree, &key); \
    } \
    V##_t* A##_get(A##_t *self, K##_t key) { \
        A##_iter_t iter = rb_tree_find(&self->tree, &key); \
        if (iter == NULL) return NULL; \
        return &iter->value; \
    } \
    void A##_remove(A##_t *self, A##_iter_t iter) { \
        K##_destroy(&iter->key); \
        V##_destroy(&iter->value); \
        rb_tree_remove(&self->tree, iter); \
        free(iter); \
    } \
    A##_iter_t A##_next(A##_t *self, A##_iter_t iter) { \
        return rb_tree_next(&self->tree, iter); \
    } \
    A##_iter_t A##_min(A##_t *self) { \
        return rb_tree_min(&self->tree); \
    } \
    A##_iter_t A##_max(A##_t *self) { \
        return rb_tree_max(&self->tree); \
    } \
    A##_iter_t A##_left(A##_iter_t iter) { \
        return rb_tree_left(iter); \
    } \
    A##_iter_t A##_right(A##_iter_t iter) { \
        return rb_tree_right(iter); \
    } \
    A##_iter_t A##_parent(A##_iter_t iter) { \
        return rb_tree_parent(iter); \
    } \
    void A##_destroy(A##_t *self) { \
        for (A##_iter_t i = A##_min(self); i != NULL;) { \
            A##_iter_t next = A##_next(self, i); \
            A##_remove(self, i); \
            i = next; \
        } \
        return destroy_rb_tree(&self->tree, NULL); \
    } \
    A##_t A##_copy(A##_t *self) { \
        A##_t ret = A##_create(); \
        for (A##_iter_t i = A##_min(self); i != NULL;) { \
            A##_iter_t next = A##_next(self, i); \
            A##_insert(&ret, K##_copy(&i->key), V##_copy(&i->value)); \
            i = next; \
        } \
        return ret; \
    } \
    A##_t A##_move(A##_t *self) { \
        A##_t ret = *self; \
        self->tree.rbh_root = NULL; \
        return ret; \
    }

struct rb_node {
    struct {
        struct rb_node *rbe_left;
        struct rb_node *rbe_right;
        struct rb_node *rbe_parent;
        int rbe_color;
    } entry;
    char content[0];
};
typedef struct rb_node RBNode;

struct rb_tree {
    RBNode *rbh_root;
    int (*cmp)(void *k1, void *k2);
    void (*augment)(void *elm);
};
typedef struct rb_tree RBTree;

void rb_tree_remove(RBTree *, void *iter);

// return a iterator
void *rb_tree_insert(RBTree *, void *treenode);
void *rb_tree_find(RBTree *, void *val);
void *rb_tree_next(RBTree *, void *iter);
void *rb_tree_min(RBTree *);
void *rb_tree_max(RBTree *);
void *rb_tree_left(void *node);
void *rb_tree_right(void *node);
void *rb_tree_parent(void *node);

void destroy_rb_tree(RBTree *, void (*freeCb)(void *));


#endif
