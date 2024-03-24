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

struct rb_node {
    struct {
        struct rb_node *rbe_left;
        struct rb_node *rbe_right;
        struct rb_node *rbe_parent;
        int rbe_color;
    } entry;
    char content[0];
};
typedef struct rb_node rb_node_t;

struct rb_tree {
    rb_node_t *rbh_root;
    int (*cmp)(void *k1, void *k2);
    void (*augment)(void *elm);
};
typedef struct rb_tree rb_tree_t;

void rb_tree_remove(rb_tree_t *, void *iter);

// return a iterator
void *rb_tree_insert(rb_tree_t *, void *treenode);
void *rb_tree_find(rb_tree_t *, void *val);
void *rb_tree_next(rb_tree_t *, void *iter);
void *rb_tree_min(rb_tree_t *);
void *rb_tree_max(rb_tree_t *);
void *rb_tree_left(void *node);
void *rb_tree_right(void *node);
void *rb_tree_parent(void *node);

void destroy_rb_tree(rb_tree_t *, void (*freeCb)(void *));

#endif

