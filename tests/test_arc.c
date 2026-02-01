#include <assert.h>
#include <stdio.h>
#include "arc.h"

/* 测试int类型的ARC */
ARC(IntArc, int)

/* 测试结构体类型的ARC */
typedef struct {
    int x;
    int y;
} Point;
ARC(PointArc, Point)

void test_basic_create_destroy() {
    IntArc arc = IntArc_create(42);
    assert(*IntArc_get(&arc) == 42);
    assert(atomic_load_explicit(&arc.ctrl->ref_count, memory_order_relaxed) == 1);
    IntArc_destroy(&arc);
    printf("test_basic_create_destroy passed\n");
}

void test_copy_increments_count() {
    IntArc arc1 = IntArc_create(100);
    IntArc arc2 = IntArc_copy(&arc1);
    
    assert(*IntArc_get(&arc1) == 100);
    assert(*IntArc_get(&arc2) == 100);
    assert(atomic_load_explicit(&arc1.ctrl->ref_count, memory_order_relaxed) == 2);
    assert(arc1.ctrl == arc2.ctrl);  // 共享同一控制块
    
    IntArc_destroy(&arc1);
    // arc1已无效，但arc2仍应有效，引用计数为1
    assert(*IntArc_get(&arc2) == 100);
    assert(atomic_load_explicit(&arc2.ctrl->ref_count, memory_order_relaxed) == 1);
    
    IntArc_destroy(&arc2);
    printf("test_copy_increments_count passed\n");
}

void test_multiple_copies() {
    IntArc arc1 = IntArc_create(10);
    IntArc arc2 = IntArc_copy(&arc1);
    IntArc arc3 = IntArc_copy(&arc2);
    
    assert(atomic_load(&arc1.ctrl->ref_count) == 3);
    
    IntArc_destroy(&arc1);
    assert(atomic_load(&arc2.ctrl->ref_count) == 2);
    
    IntArc_destroy(&arc2);
    assert(atomic_load(&arc3.ctrl->ref_count) == 1);
    
    IntArc_destroy(&arc3);
    printf("test_multiple_copies passed\n");
}

void test_struct_data() {
    Point p = {3, 4};
    PointArc arc = PointArc_create(p);
    
    assert(PointArc_get(&arc)->x == 3);
    assert(PointArc_get(&arc)->y == 4);
    
    // 修改通过指针应影响原始数据
    PointArc_get(&arc)->x = 10;
    assert(PointArc_get(&arc)->x == 10);
    
    PointArc arc2 = PointArc_copy(&arc);
    // 共享数据，修改arc2会影响arc
    PointArc_get(&arc2)->y = 20;
    assert(PointArc_get(&arc)->y == 20);
    
    PointArc_destroy(&arc);
    PointArc_destroy(&arc2);
    printf("test_struct_data passed\n");
}

void test_move_semantics() {
    IntArc arc1 = IntArc_create(99);
    assert(atomic_load(&arc1.ctrl->ref_count) == 1);
    
    IntArc arc2 = IntArc_move(&arc1);
    
    // arc1应被置空
    assert(arc1.ptr == NULL);
    assert(arc1.ctrl == NULL);
    
    // arc2拥有资源，引用计数不变
    assert(*IntArc_get(&arc2) == 99);
    assert(atomic_load(&arc2.ctrl->ref_count) == 1);
    
    // 安全销毁空arc1（不应崩溃）
    IntArc_destroy(&arc1);
    
    IntArc_destroy(&arc2);
    printf("test_move_semantics passed\n");
}

void test_double_destroy_safe() {
    IntArc arc = IntArc_create(5);
    IntArc_destroy(&arc);
    // 再次销毁不应崩溃（有NULL检查）
    IntArc_destroy(&arc);
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