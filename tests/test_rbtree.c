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

// 测试 TREE_MAP 的 _copy 方法
void tree_map_copy_test() {
    printf("  testing tree_map copy...\n");
    
    i32map_t tree = i32map_create();
    
    // 插入测试数据
    for (int32_t i = 1; i <= 5; i++) {
        i32map_insert(&tree, i, i * 100);
    }
    
    // 执行复制
    i32map_t tree_copy = i32map_copy(&tree);
    
    // 验证复制后的内容正确
    for (int32_t i = 1; i <= 5; i++) {
        int32_t *val = i32map_get(&tree_copy, i);
        assert(val != NULL);
        assert(*val == i * 100);
    }
    
    // 验证两个树是独立的：修改原树不影响复制树
    // 1. 向原树添加新元素
    i32map_insert(&tree, 10, 1000);
    assert(i32map_get(&tree, 10) != NULL);
    assert(i32map_get(&tree_copy, 10) == NULL);  // 复制树不应有此元素
    
    // 2. 从原树删除元素，复制树应保留
    i32map_iter_t iter = i32map_find(&tree, 3);
    assert(iter != NULL);
    i32map_remove(&tree, iter);
    
    assert(i32map_get(&tree, 3) == NULL);  // 原树已删除
    assert(i32map_get(&tree_copy, 3) != NULL);  // 复制树仍存在
    assert(*i32map_get(&tree_copy, 3) == 300);
    
    // 验证复制树的遍历完整性
    int count = 0;
    for (i32map_iter_t it = i32map_min(&tree_copy); it != NULL; it = i32map_next(&tree_copy, it)) {
        count++;
    }
    assert(count == 5);  // 复制树应有5个元素（1,2,3,4,5）
    
    // 验证原树的遍历（只有4个元素：1,2,4,5,10）
    count = 0;
    for (i32map_iter_t it = i32map_min(&tree); it != NULL; it = i32map_next(&tree, it)) {
        count++;
    }
    assert(count == 5);  // 1,2,4,5,10
    
    // 清理资源
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
