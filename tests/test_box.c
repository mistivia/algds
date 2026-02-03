#include <assert.h>
#include <stdio.h>
#include "box.h"

/* Test Box with int type */
BOX(IntBox, int)

/* Test Box with struct type */
typedef struct {
    int x;
    int y;
} Point;

BOX(PointBox, Point)

void test_basic_create_destroy() {
    IntBox box = IntBox_create(42);
    assert(*IntBox_get(&box) == 42);
    IntBox_destroy(&box);
    printf("test_basic_create_destroy passed\n");
}

void test_move_semantics() {
    IntBox box1 = IntBox_create(100);
    assert(*IntBox_get(&box1) == 100);
    
    // Move box1 to box2
    IntBox box2 = IntBox_move(&box1);
    
    // box1 should be null after move
    assert(box1.ptr == NULL);
    
    // box2 should own the resource
    assert(*IntBox_get(&box2) == 100);
    
    // Modifying through box2 should work
    *IntBox_get(&box2) = 200;
    assert(*IntBox_get(&box2) == 200);
    
    IntBox_destroy(&box2);
    printf("test_move_semantics passed\n");
}

void test_double_destroy_safe() {
    IntBox box = IntBox_create(5);
    IntBox_destroy(&box);
    // Second destroy should not crash (NULL check)
    IntBox_destroy(&box);
    printf("test_double_destroy_safe passed\n");
}

void test_struct_data() {
    Point p = {3, 4};
    PointBox box = PointBox_create(p);
    
    assert(PointBox_get(&box)->x == 3);
    assert(PointBox_get(&box)->y == 4);
    
    // Modify through pointer
    PointBox_get(&box)->x = 10;
    assert(PointBox_get(&box)->x == 10);
    
    PointBox_destroy(&box);
    printf("test_struct_data passed\n");
}

void test_null_get() {
    IntBox box = IntBox_create(99);
    IntBox box2 = IntBox_move(&box);
    
    // box is now empty, get should return NULL
    assert(IntBox_get(&box) == NULL);
    
    // box2 should still work
    assert(*IntBox_get(&box2) == 99);
    
    IntBox_destroy(&box);
    IntBox_destroy(&box2);
    printf("test_null_get passed\n");
}

int main() {
    printf("[TEST] Box\n");
    test_basic_create_destroy();
    test_move_semantics();
    test_double_destroy_safe();
    test_struct_data();
    test_null_get();
    printf("[PASS] Box\n");
    return 0;
}