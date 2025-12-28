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

#include "type_alias.h"

#define TREE_MAP_DEF_AS(K, V, A) \
    typedef struct { \
        RBNode rbnode; \
        K key; \
        V value; \
    } A##Node; \
    typedef A##Node *A##Iter; \
    typedef struct { \
        RBTree tree; \
    } A; \
    void A##_init(A *self); \
    A A##_create(); \
    A##Iter A##_insert(A *self, K key, V value); \
    A##Iter A##_find(A *self, K key); \
    V* A##_get(A *self, K key); \
    A##Iter A##_next(A *self, A##Iter iter); \
    A##Iter A##_min(A *self); \
    A##Iter A##_max(A *self); \
    void A##_remove(A *self, A##Iter iter); \
    A##Iter A##_left(A##Iter iter); \
    A##Iter A##_right(A##Iter iter); \
    A##Iter A##_parent(A##Iter iter); \
    void A##_free(A *self);

#define TREE_MAP_DEF(K, V) TREE_MAP_DEF_AS(K, V, K##2##V##TreeMap)

#define TREE_MAP_IMPL_AS(K, V, A) \
    static inline int A##_cmp(void *vlhs, void *vrhs) { \
        K *lhs = vlhs, *rhs = vrhs; \
        return K##_cmp(*lhs, *rhs); \
    } \
    void A##_init(A *self) { \
        self->tree.rbh_root = NULL; \
        self->tree.cmp = A##_cmp; \
        self->tree.augment = NULL; \
    } \
    A A##_create() { \
        A self; \
        A##_init(&self); \
        return self; \
    } \
    A##Iter A##_insert(A *self, K key, V value) { \
        A##Node *newnode = malloc(sizeof(A##Node)); \
        newnode->key = key; \
        newnode->value = value; \
        return rb_tree_insert(&self->tree, newnode); \
    } \
    A##Iter A##_find(A *self, K key) { \
        return rb_tree_find(&self->tree, &key); \
    } \
    V* A##_get(A *self, K key) { \
        A##Iter iter = rb_tree_find(&self->tree, &key); \
        if (iter == NULL) return NULL; \
        return &iter->value; \
    } \
    void A##_remove(A *self, A##Iter iter) { \
        rb_tree_remove(&self->tree, iter); \
    } \
    A##Iter A##_next(A *self, A##Iter iter) { \
        return rb_tree_next(&self->tree, iter); \
    } \
    A##Iter A##_min(A *self) { \
        return rb_tree_min(&self->tree); \
    } \
    A##Iter A##_max(A *self) { \
        return rb_tree_max(&self->tree); \
    } \
    A##Iter A##_left(A##Iter iter) { \
        return rb_tree_left(iter); \
    } \
    A##Iter A##_right(A##Iter iter) { \
        return rb_tree_right(iter); \
    } \
    A##Iter A##_parent(A##Iter iter) { \
        return rb_tree_parent(iter); \
    } \
    void A##_free(A *self) { \
        return destroy_rb_tree(&self->tree, NULL); \
    }

#define TREE_MAP_IMPL(K, V) TREE_MAP_IMPL_AS(K, V, K##2##V##TreeMap)

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

TREE_MAP_DEF(String, Int);
TREE_MAP_DEF(String, String);
TREE_MAP_DEF(String, Double);
TREE_MAP_DEF(String, VoidPtr);
TREE_MAP_DEF(Int, Int);
TREE_MAP_DEF(Int, Double);
TREE_MAP_DEF(VoidPtr, Int);
TREE_MAP_DEF(VoidPtr, String);
TREE_MAP_DEF(VoidPtr, VoidPtr);

#endif

