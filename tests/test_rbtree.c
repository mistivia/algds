#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "rb_tree.h"

typedef struct {
    rb_node_t node;
    int key;
    int value;
} IntIntEntry;

static int cmpfunc(void *x, void *y) {
    int *a = x, *b = y;
    return *a < *b ? -1 : *a > *b;
}

static void test_largedata();

static int max(int a, int b) { return a > b ? a : b; }

int depth(void *n) {
    rb_node_t *node = n;
    if (node == NULL) return 0;
    return max(depth(node->entry.rbe_left), depth(node->entry.rbe_right)) + 1;
}

int main() {
    printf("[TEST] rbtree\n");
    rb_tree_t tree = {NULL, cmpfunc, NULL};
    IntIntEntry *n;

    int a[5] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        n = malloc(sizeof(*n));
        n->key = a[i];
        n->value = i;
        rb_tree_insert(&tree, n);
    }

    int find = 3;
    IntIntEntry *iter;
    iter = rb_tree_find(&tree, &find);
    assert(iter->key == 3);

    rb_tree_remove(&tree, iter);
    free(iter);

    iter = rb_tree_min(&tree);
    int expected[4] = {1, 2, 4, 5};
    int i = 0;
    for (; iter != NULL; iter = rb_tree_next(&tree, iter)) {
        assert(iter->key == expected[i]);
        i++;
    }

    destroy_rb_tree(&tree, NULL);
    test_largedata();
    printf("[PASS] rbtree\n");
    return 0;
}

#define TESTSZ 10000
int input[TESTSZ];

void shuffle(int *input, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % i;
        int tmp = input[i];
        input[i] = input[j];
        input[j] = tmp;
    }
}

static void test_largedata() {
    // generate random input
    time_t t;
    srand((unsigned)time(&t));
    for (int i = 0; i < TESTSZ; i++) {
        input[i] = i;
    }
    shuffle(input, TESTSZ);
    // insert
    rb_tree_t tree = {NULL, cmpfunc, NULL};
    IntIntEntry *n;
    for (int i = 0; i < TESTSZ; i++) {
        n = malloc(sizeof(*n));
        n->key = input[i];
        n->value = input[i];
        rb_tree_insert(&tree, n);
    }
    // check tree validity
    int d = depth(tree.rbh_root);
    assert(d >= 13 && d <= 28);
    IntIntEntry *iter = rb_tree_min(&tree);
    int i = 0;
    for (; iter != NULL; iter = rb_tree_next(&tree, iter)) {
        assert(iter->key == i);
        i++;
    }
    // delete when: key % 3 != 0
    memset(input, 0, sizeof(int) * TESTSZ);
    int count = 0;
    for (int i = 0; i < TESTSZ; i++) {
        if (i % 3 != 0) {
            input[count] = i;
        } else {
            continue;
        }
        count++;
    }
    shuffle(input, count);
    for (int i = 0; i < count; i++) {
        IntIntEntry *iter = rb_tree_find(&tree, &input[i]);
        assert(iter != NULL);
        rb_tree_remove(&tree, iter);
        free(iter);
    }
    // check tree validity
    d = depth(tree.rbh_root);
    assert(d >= 11 && d <= 24);
    iter = rb_tree_min(&tree);
    i = 0;
    for (; iter != NULL; iter = rb_tree_next(&tree, iter)) {
        assert(iter->key == i * 3);
        i++;
    }
    destroy_rb_tree(&tree, NULL);
}
