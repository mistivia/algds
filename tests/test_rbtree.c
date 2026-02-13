#include "tree_map.h"
#include "basic_types.h"

#include <assert.h>
#include <stdio.h>

TREE_MAP(i32map, int32, int32)

void tree_map_basic_test() {
    i32map_t tree;
    i32map_init(&tree);

    int32_t a[5] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        i32map_insert(&tree, a[i], a[i]*2);
    }

    i32map_iter_t iter = i32map_find(&tree, 3);
    assert(iter->key == 3);
    assert(iter->value == 6);

    assert(*i32map_get(&tree, 3) == 6);

    i32map_remove(&tree, iter);

    iter = i32map_min(&tree);
    int32_t expected[4] = {1, 2, 4, 5};
    int i = 0;
    for (; iter != NULL; iter = i32map_next(&tree, iter)) {
        assert(iter->key == expected[i]);
        i++;
    }
    i32map_destroy(&tree);
}

// Test TREE_MAP _copy method
void tree_map_copy_test() {
    printf("  testing tree_map copy...\n");
    
    i32map_t tree = i32map_create();
    
    // Insert test data
    for (int32_t i = 1; i <= 5; i++) {
        i32map_insert(&tree, i, i * 100);
    }
    
    // Perform copy
    i32map_t tree_copy = i32map_copy(&tree);
    
    // Verify copied content is correct
    for (int32_t i = 1; i <= 5; i++) {
        int32_t *val = i32map_get(&tree_copy, i);
        assert(val != NULL);
        assert(*val == i * 100);
    }
    
    // Verify the two trees are independent: modifying original doesn't affect copy
    // 1. Add new element to original tree
    i32map_insert(&tree, 10, 1000);
    assert(i32map_get(&tree, 10) != NULL);
    assert(i32map_get(&tree_copy, 10) == NULL);  // copied tree should not have this element
    
    // 2. Remove element from original tree, copy should retain it
    i32map_iter_t iter = i32map_find(&tree, 3);
    assert(iter != NULL);
    i32map_remove(&tree, iter);
    
    assert(i32map_get(&tree, 3) == NULL);  // Original tree removed
    assert(i32map_get(&tree_copy, 3) != NULL);  // Copied tree still exists
    assert(*i32map_get(&tree_copy, 3) == 300);
    
    // Verify copied tree traversal integrity
    int count = 0;
    for (i32map_iter_t it = i32map_min(&tree_copy); it != NULL; it = i32map_next(&tree_copy, it)) {
        count++;
    }
    assert(count == 5);  // copied tree should have 5 elements (1,2,3,4,5)
    
    // Verify original tree traversal (5 elements: 1,2,4,5,10)
    count = 0;
    for (i32map_iter_t it = i32map_min(&tree); it != NULL; it = i32map_next(&tree, it)) {
        count++;
    }
    assert(count == 5);  // 1,2,4,5,10
    
    // Cleanup resources
    i32map_destroy(&tree);
    i32map_destroy(&tree_copy);
    
    printf("  tree_map copy test passed\n");
}

int main() {
    printf("[TEST] rbtree\n");

    tree_map_basic_test();
    tree_map_copy_test();

    printf("[PASS] rbtree\n");
    return 0;
}
