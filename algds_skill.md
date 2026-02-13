# ALGDS - C泛型数据结构库

## 快速开始

```c
#include "basic_types.h"
#include "vec.h"

VECTOR(int_vec, int32)

int main() {
    int_vec_t vec = int_vec_create();
    int_vec_push_back(&vec, 42);
    int32_t *ptr = int_vec_ref(&vec, 0);
    int_vec_destroy(&vec);
}
```

## 类型要求

自定义类型需实现以下方法才能用于容器：

```c
int      Type_cmp(Type_t *a, Type_t *b);   // 比较: <0, 0, >0
bool     Type_eq(Type_t *a, Type_t *b);    // 相等判断
uint64_t Type_hash(Type_t *x);             // 哈希函数
void     Type_destroy(Type_t *x);          // 资源清理
Type_t   Type_copy(Type_t *x);             // 深拷贝
```

**基础类型**（int32/int64/uint32/uint64/bool/char/real32/real64）已在 `basic_types.h` 预定义。


---

## 容器API速查

### Vector (动态数组) - vec.h

```c
VECTOR(name_prefix, element_type)  // 定义

name_prefix_t name_prefix_create(void);
void name_prefix_push_back(name_prefix_t *vec, element_type elem);
void name_prefix_pop(name_prefix_t *vec);
void name_prefix_insert_before(name_prefix_t *vec, int idx, element_type elem);
void name_prefix_remove(name_prefix_t *vec, int idx);
element_type_t *name_prefix_ref(name_prefix_t *vec, int idx);     // O(1)随机访问
element_type_t *name_prefix_begin(name_prefix_t *vec);
element_type_t *name_prefix_end(name_prefix_t *vec);
element_type_t *name_prefix_last(name_prefix_t *vec);
void name_prefix_swap(name_prefix_t *vec, int i, int j);
int name_prefix_len(name_prefix_t *vec);
bool name_prefix_empty(name_prefix_t *vec);
void name_prefix_destroy(name_prefix_t *vec);
name_prefix_t name_prefix_move(name_prefix_t *vec);
name_prefix_t name_prefix_copy(name_prefix_t *vec);
```

**注意：** push_back可能触发realloc，使已有指针迭代器失效。

---

### List (双向链表) - list.h

```c
LIST(name_prefix, element_type)  // 定义

name_prefix_t name_prefix_create(void);
void name_prefix_init(name_prefix_t *list);
void name_prefix_destroy(name_prefix_t *list);
name_prefix_iter_t name_prefix_push_back(name_prefix_t *list, element_type elem);
name_prefix_iter_t name_prefix_push_front(name_prefix_t *list, element_type elem);
void name_prefix_pop_back(name_prefix_t *list);
void name_prefix_pop_front(name_prefix_t *list);
name_prefix_iter_t name_prefix_insert_before(name_prefix_t *list, name_prefix_iter_t it, element_type elem);
name_prefix_iter_t name_prefix_insert_after(name_prefix_t *list, name_prefix_iter_t it, element_type elem);
void name_prefix_remove(name_prefix_t *list, name_prefix_iter_t it);
name_prefix_iter_t name_prefix_begin(name_prefix_t *list);
name_prefix_iter_t name_prefix_last(name_prefix_t *list);
name_prefix_iter_t name_prefix_end(name_prefix_t *list);
name_prefix_iter_t name_prefix_next(name_prefix_iter_t it);
name_prefix_iter_t name_prefix_prev(name_prefix_iter_t it);
int name_prefix_len(name_prefix_t *list);
```

**迭代器安全删除：**
```c
for (it = list_begin(&lst), next; it != list_end(&lst); it = next) {
    next = list_next(it);
    if (should_delete(it->val)) list_remove(&lst, it);
}
```

---

### Hash Table (哈希表) - hash_table.h

```c
HASH_TABLE(name_prefix, key_type, value_type)  // 定义

name_prefix_t name_prefix_create(void);
void name_prefix_destroy(name_prefix_t *ht);
void name_prefix_insert(name_prefix_t *ht, key_type key, value_type value);
value_type_t *name_prefix_get(name_prefix_t *ht, key_type key);  // NULL表示不存在
name_prefix_iter_t name_prefix_find(name_prefix_t *ht, key_type key);
void name_prefix_remove(name_prefix_t *ht, name_prefix_iter_t it);
name_prefix_iter_t name_prefix_begin(name_prefix_t *ht);
name_prefix_iter_t name_prefix_end(name_prefix_t *ht);
name_prefix_iter_t name_prefix_next(name_prefix_t *ht, name_prefix_iter_t it);
int name_prefix_len(name_prefix_t *ht);
```

**迭代器访问：** `it->key`, `it->val`

**注意：** insert可能触发rehash，使迭代器失效。

---

### Tree Map (红黑树) - tree_map.h

```c
TREE_MAP(name_prefix, key_type, value_type)  // 定义

name_prefix_t name_prefix_create(void);
void name_prefix_destroy(name_prefix_t *tree);
void name_prefix_insert(name_prefix_t *tree, key_type key, value_type value);
value_type_t *name_prefix_get(name_prefix_t *tree, key_type key);
name_prefix_iter_t name_prefix_find(name_prefix_t *tree, key_type key);
void name_prefix_remove(name_prefix_t *tree, name_prefix_iter_t it);
name_prefix_iter_t name_prefix_next(name_prefix_t *tree, name_prefix_iter_t it);  // 中序后继
name_prefix_iter_t name_prefix_min(name_prefix_t *tree);
name_prefix_iter_t name_prefix_max(name_prefix_t *tree);
name_prefix_iter_t name_prefix_left(name_prefix_iter_t it);
name_prefix_iter_t name_prefix_right(name_prefix_iter_t it);
name_prefix_iter_t name_prefix_parent(name_prefix_iter_t it);
name_prefix_t name_prefix_move(name_prefix_t *tree);
name_prefix_t name_prefix_copy(name_prefix_t *tree);
```

**迭代器访问：** `it->key`, `it->value`

**注意：** `end()` 返回NULL，与其他容器不同。

---

### Priority Queue (最大堆) - pqueue.h

```c
PQUEUE(name_prefix, element_type)  // 定义

name_prefix_t name_prefix_create(void);
void name_prefix_destroy(name_prefix_t *pq);
void name_prefix_push(name_prefix_t *pq, element_type elem);  // O(log n)
void name_prefix_pop(name_prefix_t *pq);                      // O(log n)
element_type_t *name_prefix_top(name_prefix_t *pq);           // O(1), NULL if empty
name_prefix_t name_prefix_move(name_prefix_t *pq);
name_prefix_t name_prefix_copy(name_prefix_t *pq);
```

**注意：** 默认是最大堆，top()返回最大值。

---

### Box (独占所有权) - box.h

```c
BOX(name_prefix, element_type)  // 定义

name_prefix_t name_prefix_create(element_type val);
element_type_t *name_prefix_get(name_prefix_t *box);
name_prefix_t name_prefix_move(name_prefix_t *box);  // 转移所有权
void name_prefix_destroy(name_prefix_t *box);
```

**注意：** Box不可copy（设计如此，链接时会报错）。

---

### Arc (原子引用计数) - arc.h

```c
ARC(name_prefix, element_type)  // 定义

name_prefix_t name_prefix_create(element_type val);
name_prefix_t name_prefix_copy(name_prefix_t *arc);  // 引用计数+1
element_type_t *name_prefix_get(name_prefix_t *arc);
name_prefix_t name_prefix_move(name_prefix_t *arc);  // 转移，计数不变
void name_prefix_destroy(name_prefix_t *arc);       // 计数-1，为0时释放
```

**注意：** 只有引用计数是线程安全的，数据访问仍需同步。

---

## 算法

### 快速排序 - sort.h

```c
QSORT(element_type)  // 在.c文件中调用，生成 type_qsort 函数

void type_qsort(type_t *arr, int n);
```

**预定义：** `int32_qsort`, `int64_qsort`, `uint32_qsort`, `uint64_qsort`, `real32_qsort`, `real64_qsort`

---

### 字符串工具 - str.h

```c
// 字符串处理
char *str_strip(char *str);
char **str_split(char *str, char delim);
void destroy_str_list(char **list);

// 字符串构建器
typedef struct { char *buf; int size; int capacity; } str_builder_t;
str_builder_t str_builder_create(void);
void str_builder_append(str_builder_t *sb, char *fmt, ...);
void str_builder_append_char(str_builder_t *sb, char c);
str_builder_t str_builder_move(str_builder_t *sb);
void str_builder_destroy(str_builder_t *sb);

// 文件操作
char *fgetline(FILE *fp);
int fpeek(FILE *fp);
```

---

## 常见陷阱

| 问题 | 说明 | 解决 |
|------|------|------|
| Vector迭代器失效 | push_back触发realloc后指针失效 | 操作后重新获取迭代器 |
| 未定义基础方法 | 自定义类型缺少cmp/eq/hash等 | 实现所需方法或使用BASIC_TYPE |
| 重复释放 | 直接赋值结构体后多次destroy | 使用move/copy |
| Arc数据竞争 | Arc只保证引用计数线程安全 | 数据访问需额外同步 |
| Box复制 | Box设计为不可复制 | 使用move转移所有权 |
| Tree Map的end() | 返回NULL，不能解引用 | 仅作遍历终止条件 |

---

## 完整示例

```c
#include <stdio.h>
#include "basic_types.h"
#include "vec.h"
#include "list.h"
#include "hash_table.h"
#include "pqueue.h"
#include "sort.h"

VECTOR(int_vec, int32)
LIST(int_list, int32)
HASH_TABLE(int_map, int32, int32)
PQUEUE(int_pq, int32)

int main() {
    // Vector
    int_vec_t vec = int_vec_create();
    for (int i = 0; i < 5; i++) int_vec_push_back(&vec, i);
    for (int32_t *it = int_vec_begin(&vec); it != int_vec_end(&vec); it++)
        printf("%d ", *it);
    int_vec_destroy(&vec);

    // List
    int_list_t lst = int_list_create();
    int_list_push_back(&lst, 1);
    int_list_push_back(&lst, 2);
    for (int_list_iter_t it = int_list_begin(&lst); it != int_list_end(&lst); it = int_list_next(it))
        printf("%d ", it->val);
    int_list_destroy(&lst);

    // Hash Table
    int_map_t ht = int_map_create();
    int_map_insert(&ht, 1, 10);
    int_map_insert(&ht, 2, 20);
    int32_t *val = int_map_get(&ht, 1);
    int_map_destroy(&ht);

    // Priority Queue
    int_pq_t pq = int_pq_create();
    int_pq_push(&pq, 5);
    int_pq_push(&pq, 10);
    while (int_pq_top(&pq) != NULL) {
        printf("%d ", *int_pq_top(&pq));
        int_pq_pop(&pq);
    }
    int_pq_destroy(&pq);

    // Sort
    int32_t arr[] = {5, 2, 8, 1, 9};
    int32_qsort(arr, 5);

    return 0;
}
```
