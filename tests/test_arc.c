#include <assert.h>
#include <stdio.h>
#include "arc.h"


/* 测试结构体类型 */
typedef struct {
    int32_t x;
    int32_t y;
} point_t;

/* 实例化ARC宏 */
ARC(int_arc, int32)
ARC(point_arc, point)

void test_basic_create_destroy() {
    int_arc_t arc = int_arc_create(42);
    assert(*int_arc_get(&arc) == 42);
    assert(atomic_load_explicit(&arc.ctrl->ref_count, memory_order_relaxed) == 1);
    int_arc_destroy(&arc);
    printf("test_basic_create_destroy passed\n");
}

void test_copy_increments_count() {
    int_arc_t arc1 = int_arc_create(100);
    int_arc_t arc2 = int_arc_copy(&arc1);
    
    assert(*int_arc_get(&arc1) == 100);
    assert(*int_arc_get(&arc2) == 100);
    assert(atomic_load_explicit(&arc1.ctrl->ref_count, memory_order_relaxed) == 2);
    assert(arc1.ctrl == arc2.ctrl);  // 共享同一控制块
    
    int_arc_destroy(&arc1);
    // arc1已无效，但arc2仍应有效，引用计数为1
    assert(*int_arc_get(&arc2) == 100);
    assert(atomic_load_explicit(&arc2.ctrl->ref_count, memory_order_relaxed) == 1);
    
    int_arc_destroy(&arc2);
    printf("test_copy_increments_count passed\n");
}

void test_multiple_copies() {
    int_arc_t arc1 = int_arc_create(10);
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
    point_t p = {3, 4};
    point_arc_t arc = point_arc_create(p);
    
    assert(point_arc_get(&arc)->x == 3);
    assert(point_arc_get(&arc)->y == 4);
    
    // 修改通过指针应影响原始数据
    point_arc_get(&arc)->x = 10;
    assert(point_arc_get(&arc)->x == 10);
    
    point_arc_t arc2 = point_arc_copy(&arc);
    // 共享数据，修改arc2会影响arc
    point_arc_get(&arc2)->y = 20;
    assert(point_arc_get(&arc)->y == 20);
    
    point_arc_destroy(&arc);
    point_arc_destroy(&arc2);
    printf("test_struct_data passed\n");
}

void test_move_semantics() {
    int_arc_t arc1 = int_arc_create(99);
    assert(atomic_load(&arc1.ctrl->ref_count) == 1);
    
    int_arc_t arc2 = int_arc_move(&arc1);
    
    // arc1应被置空
    assert(arc1.ptr == NULL);
    assert(arc1.ctrl == NULL);
    
    // arc2拥有资源，引用计数不变
    assert(*int_arc_get(&arc2) == 99);
    assert(atomic_load(&arc2.ctrl->ref_count) == 1);
    
    // 安全销毁空arc1（不应崩溃）
    int_arc_destroy(&arc1);
    
    int_arc_destroy(&arc2);
    printf("test_move_semantics passed\n");
}

void test_double_destroy_safe() {
    int_arc_t arc = int_arc_create(5);
    int_arc_destroy(&arc);
    // 再次销毁不应崩溃（有NULL检查）
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
