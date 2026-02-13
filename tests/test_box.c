#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include "box.h"

/* Test Box with int32 type */
BOX(int32_box, int32)

/* Test Box with struct type */
typedef struct {
    int x;
    int y;
} point_t;

BOX(point_box, point)

void test_basic_create_destroy() {
    int32_box_t box = int32_box_create(42);
    assert(*int32_box_get(&box) == 42);
    int32_box_destroy(&box);
    printf("test_basic_create_destroy passed\n");
}

void test_move_semantics() {
    int32_box_t box1 = int32_box_create(100);
    assert(*int32_box_get(&box1) == 100);
    
    // Move box1 to box2
    int32_box_t box2 = int32_box_move(&box1);
    
    // box1 should be null after move
    assert(box1.ptr == NULL);
    
    // box2 should own the resource
    assert(*int32_box_get(&box2) == 100);
    
    // Modifying through box2 should work
    *int32_box_get(&box2) = 200;
    assert(*int32_box_get(&box2) == 200);
    
    int32_box_destroy(&box2);
    printf("test_move_semantics passed\n");
}

void test_double_destroy_safe() {
    int32_box_t box = int32_box_create(5);
    int32_box_destroy(&box);
    // Second destroy should not crash (NULL check)
    int32_box_destroy(&box);
    printf("test_double_destroy_safe passed\n");
}

void test_struct_data() {
    point_t p = {3, 4};
    point_box_t box = point_box_create(p);
    
    assert(point_box_get(&box)->x == 3);
    assert(point_box_get(&box)->y == 4);
    
    // Modify through pointer
    point_box_get(&box)->x = 10;
    assert(point_box_get(&box)->x == 10);
    
    point_box_destroy(&box);
    printf("test_struct_data passed\n");
}

void test_null_get() {
    int32_box_t box = int32_box_create(99);
    int32_box_t box2 = int32_box_move(&box);
    
    // box is now empty, get should return NULL
    assert(int32_box_get(&box) == NULL);
    
    // box2 should still work
    assert(*int32_box_get(&box2) == 99);
    
    int32_box_destroy(&box);
    int32_box_destroy(&box2);
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
