#include <assert.h>
#include <stdio.h>
#include "arc.h"


/* Test struct type */
typedef struct {
    int32_t x;
    int32_t y;
} point_t;

/* Instantiate ARC macro */
ARC(int_arc, int32)
ARC(point_arc, point)

void test_basic_create_destroy() {
    int32_t* val = malloc(sizeof(int32_t));
    *val = 42;
    int_arc_t arc = int_arc_create(val);
    assert(*int_arc_get(&arc) == 42);
    assert(atomic_load_explicit(&arc.ctrl->ref_count, memory_order_relaxed) == 1);
    int_arc_destroy(&arc);
    printf("test_basic_create_destroy passed\n");
}

void test_copy_increments_count() {
    int32_t* val = malloc(sizeof(int32_t));
    *val = 100;
    int_arc_t arc1 = int_arc_create(val);
    int_arc_t arc2 = int_arc_copy(&arc1);
    
    assert(*int_arc_get(&arc1) == 100);
    assert(*int_arc_get(&arc2) == 100);
    assert(atomic_load_explicit(&arc1.ctrl->ref_count, memory_order_relaxed) == 2);
    assert(arc1.ctrl == arc2.ctrl);  // Share the same control block
    
    int_arc_destroy(&arc1);
    // arc1 is invalid, but arc2 should still be valid, ref count is 1
    assert(*int_arc_get(&arc2) == 100);
    assert(atomic_load_explicit(&arc2.ctrl->ref_count, memory_order_relaxed) == 1);
    
    int_arc_destroy(&arc2);
    printf("test_copy_increments_count passed\n");
}

void test_multiple_copies() {
    int32_t* val = malloc(sizeof(int32_t));
    *val = 10;
    int_arc_t arc1 = int_arc_create(val);
    int_arc_t arc2 = int_arc_copy(&arc1);
    int_arc_t arc3 = int_arc_copy(&arc2);
    
    assert(atomic_load(&arc1.ctrl->ref_count) == 3);
    
    int_arc_destroy(&arc1);
    assert(atomic_load(&arc2.ctrl->ref_count) == 2);
    
    int_arc_destroy(&arc2);
    assert(atomic_load(&arc3.ctrl->ref_count) == 1);
    
    int_arc_destroy(&arc3);
    printf("test_multiple_copies passed\n");
}

void test_struct_data() {
    point_t* p = malloc(sizeof(point_t));
    p->x = 3;
    p->y = 4;
    point_arc_t arc = point_arc_create(p);
    
    assert(point_arc_get(&arc)->x == 3);
    assert(point_arc_get(&arc)->y == 4);
    
    // Modification through pointer should affect original data
    point_arc_get(&arc)->x = 10;
    assert(point_arc_get(&arc)->x == 10);
    
    point_arc_t arc2 = point_arc_copy(&arc);
    // Shared data, modifying arc2 affects arc
    point_arc_get(&arc2)->y = 20;
    assert(point_arc_get(&arc)->y == 20);
    
    point_arc_destroy(&arc);
    point_arc_destroy(&arc2);
    printf("test_struct_data passed\n");
}

void test_move_semantics() {
    int32_t* val = malloc(sizeof(int32_t));
    *val = 99;
    int_arc_t arc1 = int_arc_create(val);
    assert(atomic_load(&arc1.ctrl->ref_count) == 1);
    
    int_arc_t arc2 = int_arc_move(&arc1);
    
    // arc1 should be set to null
    assert(arc1.ptr == NULL);
    assert(arc1.ctrl == NULL);
    
    // arc2 owns the resource, ref count unchanged
    assert(*int_arc_get(&arc2) == 99);
    assert(atomic_load(&arc2.ctrl->ref_count) == 1);
    
    // Safely destroy empty arc1 (should not crash)
    int_arc_destroy(&arc1);
    
    int_arc_destroy(&arc2);
    printf("test_move_semantics passed\n");
}

void test_double_destroy_safe() {
    int32_t* val = malloc(sizeof(int32_t));
    *val = 5;
    int_arc_t arc = int_arc_create(val);
    int_arc_destroy(&arc);
    // Destroying again should not crash (has NULL check)
    int_arc_destroy(&arc);
    printf("test_double_destroy_safe passed\n");
}

int main() {
    printf("[TEST] Arc\n");
    test_basic_create_destroy();
    test_copy_increments_count();
    test_multiple_copies();
    test_struct_data();
    test_move_semantics();
    test_double_destroy_safe();
    printf("[PASS] Arc\n");
    return 0;
}
