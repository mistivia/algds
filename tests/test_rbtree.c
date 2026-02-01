#include <assert.h>
#include <stdio.h>

#include "tree_map.h"
#include "basic_types.h"

TREE_MAP(Int2IntTreeMap, Int, Int)

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

    iter = Int2IntTreeMap_min(&tree);
    int expected[4] = {1, 2, 4, 5};
    int i = 0;
    for (; iter != NULL; iter = Int2IntTreeMap_next(&tree, iter)) {
        assert(iter->key == expected[i]);
        i++;
    }
    Int2IntTreeMap_destroy(&tree);
}

// 测试 TREE_MAP 的 _copy 方法
void tree_map_copy_test() {
    printf("  testing tree_map copy...\n");
    
    Int2IntTreeMap tree = Int2IntTreeMap_create();
    
    // 插入测试数据
    for (int i = 1; i <= 5; i++) {
        Int2IntTreeMap_insert(&tree, i, i * 100);
    }
    
    // 执行复制
    Int2IntTreeMap tree_copy = Int2IntTreeMap_copy(&tree);
    
    // 验证复制后的内容正确
    for (int i = 1; i <= 5; i++) {
        Int *val = Int2IntTreeMap_get(&tree_copy, i);
        assert(val != NULL);
        assert(*val == i * 100);
    }
    
    // 验证两个树是独立的：修改原树不影响复制树
    // 1. 向原树添加新元素
    Int2IntTreeMap_insert(&tree, 10, 1000);
    assert(Int2IntTreeMap_get(&tree, 10) != NULL);
    assert(Int2IntTreeMap_get(&tree_copy, 10) == NULL);  // 复制树不应有此元素
    
    // 2. 从原树删除元素，复制树应保留
    Int2IntTreeMapIter iter = Int2IntTreeMap_find(&tree, 3);
    assert(iter != NULL);
    Int2IntTreeMap_remove(&tree, iter);
    
    assert(Int2IntTreeMap_get(&tree, 3) == NULL);  // 原树已删除
    assert(Int2IntTreeMap_get(&tree_copy, 3) != NULL);  // 复制树仍存在
    assert(*Int2IntTreeMap_get(&tree_copy, 3) == 300);
    
    // 验证复制树的遍历完整性
    int count = 0;
    for (Int2IntTreeMapIter it = Int2IntTreeMap_min(&tree_copy); it != NULL; it = Int2IntTreeMap_next(&tree_copy, it)) {
        count++;
    }
    assert(count == 5);  // 复制树应有5个元素（1,2,3,4,5）
    
    // 验证原树的遍历（只有4个元素：1,2,4,5,10）
    count = 0;
    for (Int2IntTreeMapIter it = Int2IntTreeMap_min(&tree); it != NULL; it = Int2IntTreeMap_next(&tree, it)) {
        count++;
    }
    assert(count == 5);  // 1,2,4,5,10
    
    // 清理资源
    Int2IntTreeMap_destroy(&tree);
    Int2IntTreeMap_destroy(&tree_copy);
    
    printf("  tree_map copy test passed\n");
}

int main() {
    printf("[TEST] rbtree\n");

    tree_map_basic_test();
    tree_map_copy_test();

    printf("[PASS] rbtree\n");
    return 0;
}
