#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "tree_map.h"

typedef struct {
    RBNode node;
    int key;
    int value;
} Int2IntRBNode;

static int cmpfunc(void *x, void *y) {
    int *a = x, *b = y;
    return *a < *b ? -1 : *a > *b;
}

static void test_largedata();

static int max(int a, int b) { return a > b ? a : b; }

int depth(void *n) {
    RBNode *node = n;
    if (node == NULL) return 0;
    return max(depth(node->entry.rbe_left), depth(node->entry.rbe_right)) + 1;
}

void basic_test() {
    RBTree tree = {NULL, cmpfunc, NULL};
    Int2IntRBNode *n;

    int a[5] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        n = malloc(sizeof(*n));
        n->key = a[i];
        n->value = i;
        rb_tree_insert(&tree, n);
    }

    int find = 3;
    Int2IntRBNode *iter;
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
}

void tree_map_basic_test() {
    Int2IntTreeMap tree;
    Int2IntTreeMap_init(&tree);

    int a[5] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        Int2IntTreeMap_insert(&tree, a[i], a[i]*2);
    }

    Int2IntTreeMapIter iter = Int2IntTreeMap_find(&tree, 3);
    assert(iter->key == 3);
    assert(iter->value == 6);

    assert(*Int2IntTreeMap_get(&tree, 3) == 6);

    Int2IntTreeMap_remove(&tree, iter);
    free(iter);

    iter = Int2IntTreeMap_min(&tree);
    int expected[4] = {1, 2, 4, 5};
    int i = 0;
    for (; iter != NULL; iter = Int2IntTreeMap_next(&tree, iter)) {
        assert(iter->key == expected[i]);
        i++;
    }
    Int2IntTreeMap_free(&tree);
}


int main() {
    printf("[TEST] rbtree\n");

    basic_test();
    tree_map_basic_test();
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
    RBTree tree = {NULL, cmpfunc, NULL};
    Int2IntRBNode *n;
    for (int i = 0; i < TESTSZ; i++) {
        n = malloc(sizeof(*n));
        n->key = input[i];
        n->value = input[i];
        rb_tree_insert(&tree, n);
    }
    // check tree validity
    int d = depth(tree.rbh_root);
    assert(d >= 13 && d <= 28);
    Int2IntRBNode *iter = rb_tree_min(&tree);
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
        Int2IntRBNode *iter = rb_tree_find(&tree, &input[i]);
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
