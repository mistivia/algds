# ALGDS Manual - C语言泛型数据结构与算法库

## 目录

1. [概述](#概述)
2. [核心概念](#核心概念)
3. [基本类型系统](#基本类型系统)
4. [容器数据结构](#容器数据结构)
   - [Vector (动态数组)](#vector-动态数组)
   - [List (双向链表)](#list-双向链表)
   - [Hash Table (哈希表)](#hash-table-哈希表)
   - [Tree Map (红黑树)](#tree-map-红黑树)
   - [Priority Queue (优先队列)](#priority-queue-优先队列)
5. [智能指针](#智能指针)
   - [Box (独占所有权)](#box-独占所有权)
   - [Arc (原子引用计数)](#arc-原子引用计数)
6. [算法](#算法)
   - [快速排序](#快速排序)
   - [字符串工具](#字符串工具)
7. [工具宏](#工具宏)
8. [常见陷阱](#常见陷阱)
9. [设计模式](#设计模式)

---

## 概述

ALGDS (Algorithms and Data Structures) 是一个为C语言设计的泛型、类型安全的数据结构与算法库。通过C预处理器宏实现泛型编程，提供类似C++ STL的接口风格。

### 主要特性

- **类型安全**: 编译期类型检查，避免void*的内存安全问题
- **零运行时开销**: 纯宏实现，无动态分发
- **统一接口**: 所有容器遵循一致的命名和使用模式
- **RAII支持**: 完整的生命周期管理 (create/init/destroy/move/copy)

---

## 核心概念

### 类型命名约定

所有类型使用 `_t` 后缀：
- 基本类型: `int32_t`, `int64_t`, `bool_t`, `char_t`, `real32_t`, `real64_t`
- 容器类型: 由宏参数 A 生成 `A##_t`
- 迭代器类型: 由宏参数 A 生成 `A##_iter_t`

### 必需的类型方法

自定义类型需要实现以下方法才能用于容器：

```c
// 比较操作（用于排序、树结构）
int   Type_cmp(Type_t *lhs, Type_t *rhs);     // <0: lhs<rhs, 0:相等, >0: lhs>rhs

// 相等性判断（用于哈希表）
bool  Type_eq(const Type_t *lhs, const Type_t *rhs);

// 哈希函数（用于哈希表）
uint64_t Type_hash(Type_t *x);

// 资源管理
void  Type_destroy(Type_t *x);                 // 清理资源
Type_t Type_copy(Type_t *x);                   // 深拷贝
```

### 基本类型自动支持

`basic_types.h` 已为以下类型预定义了所有必需方法：
- `int32`, `int64`, `uint32`, `uint64`
- `bool`, `char`
- `real32` (float), `real64` (double)

---

## 基本类型系统

### BASIC_TYPE 宏

```c
BASIC_TYPE(my_type);
```

自动生成基本类型的比较、哈希、销毁、拷贝方法。

**示例:**
```c
#include "basic_types.h"

typedef int my_id_t;
BASIC_TYPE(my_id);  // 生成 my_id_cmp, my_id_eq, my_id_hash 等
```

---

## 容器数据结构

### Vector (动态数组)

基于动态数组的连续内存容器，支持O(1)随机访问。

**定义:**
```c
VECTOR(NamePrefix, ElementType)
```

**核心API:**

| 函数 | 说明 | 复杂度 |
|------|------|--------|
| `NamePrefix_create()` | 创建空vector | O(1) |
| `NamePrefix_push_back(vec, elem)` | 尾部添加 | 均摊O(1) |
| `NamePrefix_pop(vec)` | 尾部移除 | O(1) |
| `NamePrefix_insert_before(vec, idx, elem)` | 指定位置插入 | O(n) |
| `NamePrefix_remove(vec, idx)` | 删除指定位置 | O(n) |
| `NamePrefix_ref(vec, idx)` | 获取元素指针 | O(1) |
| `NamePrefix_begin/end/last(vec)` | 迭代器 | O(1) |
| `NamePrefix_swap(vec, i, j)` | 交换元素 | O(1) |
| `NamePrefix_len(vec)` | 获取长度 | O(1) |
| `NamePrefix_empty(vec)` | 判断是否为空 | O(1) |
| `NamePrefix_destroy(vec)` | 销毁 | O(n) |
| `NamePrefix_move/copy(vec)` | 移动/深拷贝 | O(n) |

**示例:**
```c
#include "vec.h"
#include "basic_types.h"

VECTOR(int_vec, int32)  // 定义 int32 的vector

int main() {
    int_vec_t vec = int_vec_create();
    
    // 添加元素
    for (int i = 0; i < 100; i++) {
        int_vec_push_back(&vec, i);
    }
    
    // 随机访问
    int_vec_ref(&vec, 50);  // 获取第50个元素的指针
    
    // 遍历
    for (int32_t *it = int_vec_begin(&vec); 
         it != int_vec_end(&vec); 
         it++) {
        printf("%d ", *it);
    }
    
    int_vec_destroy(&vec);
    return 0;
}
```

**典型模式 - 作为栈使用:**
```c
// push
vec_push_back(&stack, elem);

// pop
if (!vec_empty(&stack)) {
    vec_pop(&stack);
}

// top
int32_t *top = vec_last(&stack);
```

**⚠️ 陷阱:** Vector的迭代器是指针，在push_back后可能失效（realloc后内存重新分配）。

---

### List (双向链表)

带头尾哨兵节点的双向链表，支持O(1)任意位置插入删除。

**定义:**
```c
LIST(NamePrefix, ElementType)
```

**核心API:**

| 函数 | 说明 | 复杂度 |
|------|------|--------|
| `NamePrefix_create/init/destroy` | 生命周期管理 | - |
| `NamePrefix_push_back/front(list, elem)` | 头尾插入 | O(1) |
| `NamePrefix_pop_back/front(list)` | 头尾删除 | O(1) |
| `NamePrefix_insert_before/after(list, iter, elem)` | 迭代器位置插入 | O(1) |
| `NamePrefix_remove(list, iter)` | 删除迭代器指向元素 | O(1) |
| `NamePrefix_begin/last/end(list)` | 获取迭代器 | O(1) |
| `NamePrefix_next/prev(iter)` | 迭代器移动 | O(1) |
| `NamePrefix_len(list)` | 获取长度 | O(1) |

**示例:**
```c
#include "list.h"
#include "basic_types.h"

LIST(int_list, int32)

int main() {
    int_list_t lst = int_list_create();
    
    // 插入元素: 1 -> 2 -> 3
    int_list_push_back(&lst, 1);
    int_list_iter_t it = int_list_push_back(&lst, 3);
    int_list_insert_before(&lst, it, 2);
    
    // 遍历
    for (int_list_iter_t it = int_list_begin(&lst);
         it != int_list_end(&lst);
         it = int_list_next(it)) {
        printf("%d ", it->val);
    }
    
    // 删除
    int_list_pop_front(&lst);  // 删除1
    
    int_list_destroy(&lst);
    return 0;
}
```

**典型模式 - 队列:**
```c
// 入队
list_push_back(&queue, elem);

// 出队
if (list_len(&queue) > 0) {
    list_pop_front(&queue);
}

// 查看队首
int32_t *front = &list_begin(&queue)->val;
```

**典型模式 - 迭代器安全删除:**
```c
for (int_list_iter_t it = list_begin(&lst), next;
     it != list_end(&lst);
     it = next) {
    next = list_next(it);  // 先保存next
    if (should_delete(it->val)) {
        list_remove(&lst, it);  // 安全删除
    }
}
```

---

### Hash Table (哈希表)

基于开放寻址法的泛型哈希表，支持自动扩容。

**定义:**
```c
HASH_TABLE(NamePrefix, KeyType, ValueType)
```

**核心API:**

| 函数 | 说明 | 复杂度 |
|------|------|--------|
| `NamePrefix_create/init/destroy` | 生命周期管理 | - |
| `NamePrefix_insert(ht, key, value)` | 插入键值对 | 均摊O(1) |
| `NamePrefix_get(ht, key)` | 按键查找值指针 | O(1) |
| `NamePrefix_find(ht, key)` | 查找返回迭代器 | O(1) |
| `NamePrefix_remove(ht, iter)` | 删除迭代器指向项 | O(1) |
| `NamePrefix_begin/end/next(ht, iter)` | 遍历 | O(n) |
| `NamePrefix_len(ht)` | 元素数量 | O(1) |

**示例:**
```c
#include "hash_table.h"
#include "basic_types.h"

HASH_TABLE(int_map, int32, int32)  // int32 -> int32 的映射

int main() {
    int_map_t ht = int_map_create();
    
    // 插入
    for (int i = 0; i < 1000; i++) {
        int_map_insert(&ht, i, i * i);
    }
    
    // 查找
    int32_t *val = int_map_get(&ht, 50);
    if (val != NULL) {
        printf("50^2 = %d\n", *val);
    }
    
    // 遍历
    for (int_map_iter_t it = int_map_begin(&ht);
         it != int_map_end(&ht);
         it = int_map_next(&ht, it)) {
        printf("%d -> %d\n", it->key, it->val);
    }
    
    // 删除
    int_map_iter_t it = int_map_find(&ht, 50);
    if (it != int_map_end(&ht)) {
        int_map_remove(&ht, it);
    }
    
    int_map_destroy(&ht);
    return 0;
}
```

**典型模式 - 计数器:**
```c
HASH_TABLE(counter, str_t, int32)

void count_word(counter_t *cnt, str_t word) {
    int32_t *val = counter_get(cnt, word);
    if (val == NULL) {
        counter_insert(cnt, str_copy(&word), 1);
    } else {
        (*val)++;
    }
}
```

**⚠️ 陷阱:** 迭代器在insert操作后可能失效（rehash时）。

---

### Tree Map (红黑树)

基于红黑树实现的有序键值对容器。

**定义:**
```c
TREE_MAP(NamePrefix, KeyType, ValueType)
```

**核心API:**

| 函数 | 说明 | 复杂度 |
|------|------|--------|
| `NamePrefix_create/init/destroy` | 生命周期管理 | - |
| `NamePrefix_insert(tree, key, value)` | 插入 | O(log n) |
| `NamePrefix_get(tree, key)` | 查找值指针 | O(log n) |
| `NamePrefix_find(tree, key)` | 查找返回迭代器 | O(log n) |
| `NamePrefix_remove(tree, iter)` | 删除 | O(log n) |
| `NamePrefix_next(tree, iter)` | 后继迭代器 | O(log n) |
| `NamePrefix_min/max(tree)` | 最小/最大元素 | O(log n) |
| `NamePrefix_left/right/parent(iter)` | 树操作 | O(1) |
| `NamePrefix_copy/move(tree)` | 深拷贝/移动 | O(n) |

**示例:**
```c
#include "tree_map.h"
#include "basic_types.h"

TREE_MAP(int_tree, int32, int32)

int main() {
    int_tree_t tree = int_tree_create();
    
    // 插入
    for (int i = 0; i < 100; i++) {
        int_tree_insert(&tree, i, i * 2);
    }
    
    // 查找
    int32_t *val = int_tree_get(&tree, 50);
    
    // 顺序遍历（中序遍历）
    for (int_tree_iter_t it = int_tree_min(&tree);
         it != NULL;
         it = int_tree_next(&tree, it)) {
        printf("%d -> %d\n", it->key, it->value);
    }
    
    // 获取最小/最大
    int_tree_iter_t min = int_tree_min(&tree);
    int_tree_iter_t max = int_tree_max(&tree);
    
    int_tree_destroy(&tree);
    return 0;
}
```

**典型模式 - 范围查询:**
```c
// 找到>=lower_bound的第一个元素
int_tree_iter_t lower_bound(int_tree_t *tree, int32_t key) {
    int_tree_iter_t it = int_tree_find(tree, key);
    if (it != NULL) return it;
    // 如果不存在，找下一个更大的
    // ...需要自定义实现...
}

// 遍历某个范围 [start, end]
for (int_tree_iter_t it = find_or_next(&tree, start);
     it != NULL && it->key <= end;
     it = int_tree_next(&tree, it)) {
    // 处理范围内的元素
}
```

**⚠️ 陷阱:** `end()` 返回NULL作为哨兵，与其他容器的end()不一致。

---

### Priority Queue (优先队列)

基于二叉堆实现的最大堆。

**定义:**
```c
PQUEUE(NamePrefix, ElementType)
```

**核心API:**

| 函数 | 说明 | 复杂度 |
|------|------|--------|
| `NamePrefix_create/init/destroy` | 生命周期管理 | - |
| `NamePrefix_push(pq, elem)` | 入队 | O(log n) |
| `NamePrefix_pop(pq)` | 出队（最大值） | O(log n) |
| `NamePrefix_top(pq)` | 查看最大值 | O(1) |
| `NamePrefix_copy/move(pq)` | 深拷贝/移动 | O(n) |

**示例:**
```c
#include "pqueue.h"
#include "basic_types.h"

PQUEUE(int_pq, int32)

int main() {
    int_pq_t pq = int_pq_create();
    
    // 入队
    int_pq_push(&pq, 5);
    int_pq_push(&pq, 10);
    int_pq_push(&pq, 3);
    
    // 按降序出队
    while (int_pq_top(&pq) != NULL) {
        printf("%d ", *int_pq_top(&pq));
        int_pq_pop(&pq);
    }
    // 输出: 10 5 3
    
    int_pq_destroy(&pq);
    return 0;
}
```

**典型模式 - Top-K 问题:**
```c
// 找出数组中最大的K个元素
PQUEUE(min_pq, int32)  // 实际上用最小堆找最大的K个需要自定义比较器

// 或者：维护大小为K的最小堆
// (需要自定义比较器使最小堆实现，本库默认最大堆)
```

**⚠️ 陷阱:** 本库实现的是最大堆（大顶堆），`top()` 返回最大值。

---

## 智能指针

### Box (独占所有权)

堆分配的单所有权智能指针，类似Rust的Box和C++的unique_ptr。

**定义:**
```c
BOX(NamePrefix, ElementType)
```

**核心API:**

| 函数 | 说明 |
|------|------|
| `NamePrefix_create(val)` | 创建Box |
| `NamePrefix_get(box)` | 获取内部指针 |
| `NamePrefix_move(box)` | 转移所有权 |
| `NamePrefix_destroy(box)` | 销毁（释放内存） |

**特性:**
- **独占所有权**: 同一时间只有一个Box指向某块内存
- **不可复制**: `copy` 方法被声明为外部符号，链接时会报错（设计如此）
- **自动释放**: destroy时自动free

**示例:**
```c
#include "box.h"
#include "basic_types.h"

BOX(int_box, int32)

int main() {
    // 创建Box
    int_box_t box = int_box_create(42);
    
    // 获取值
    int32_t *ptr = int_box_get(&box);
    printf("%d\n", *ptr);  // 42
    
    // 转移所有权
    int_box_t box2 = int_box_move(&box);
    // 现在 box.ptr == NULL，box2 拥有所有权
    
    // 以下代码会链接失败（有意为之）
    // int_box_t box3 = int_box_copy(&box2);  // ERROR!
    
    int_box_destroy(&box2);
    return 0;
}
```

**典型模式 - 工厂函数:**
```c
my_type_t *create_object() {
    my_type_box_t box = my_type_box_create(my_type_create(args));
    return my_type_box_move(&box).ptr;  // 转移所有权给调用者
}
```

---

### Arc (原子引用计数)

线程安全的引用计数智能指针，类似Rust的Arc和C++的shared_ptr。

**定义:**
```c
ARC(NamePrefix, ElementType)
```

**核心API:**

| 函数 | 说明 |
|------|------|
| `NamePrefix_create(val)` | 创建Arc（引用计数=1） |
| `NamePrefix_copy(arc)` | 增加引用计数，返回新Arc |
| `NamePrefix_get(arc)` | 获取内部指针 |
| `NamePrefix_move(arc)` | 转移所有权（不改变引用计数） |
| `NamePrefix_destroy(arc)` | 减少引用计数，为0时释放 |

**特性:**
- **线程安全**: 使用 `atomic_int` 和内存序保证
- **共享所有权**: 多个Arc可以指向同一块数据
- **自动生命周期**: 最后一个Arc销毁时自动释放

**示例:**
```c
#include "arc.h"
#include "basic_types.h"
#include "vec.h"

ARC(int_arc, int32)
VECTOR(int_arc_vec, int_arc)

int main() {
    // 创建Arc
    int_arc_t arc1 = int_arc_create(100);
    
    // 复制引用（计数=2）
    int_arc_t arc2 = int_arc_copy(&arc1);
    
    // 存储在容器中
    int_arc_vec_t vec = int_arc_vec_create();
    int_arc_vec_push_back(&vec, int_arc_copy(&arc1));  // 计数=3
    
    // 获取值
    printf("%d\n", *int_arc_get(&arc1));  // 100
    
    // 销毁引用
    int_arc_destroy(&arc2);  // 计数=2
    int_arc_vec_destroy(&vec);  // 计数=1
    int_arc_destroy(&arc1);  // 计数=0，内存释放
    
    return 0;
}
```

**典型模式 - 共享配置:**
```c
ARC(config_arc, app_config_t)

typedef struct {
    config_arc_t shared_config;
    // ...其他字段
} worker_thread_t;

worker_thread_t *worker_create(config_arc_t *cfg) {
    worker_thread_t *w = malloc(sizeof(*w));
    w->shared_config = config_arc_copy(cfg);  // 增加引用
    return w;
}

void worker_destroy(worker_thread_t *w) {
    config_arc_destroy(&w->shared_config);  // 减少引用
    free(w);
}
```

**⚠️ 陷阱:**
- Arc内部数据**不是**线程安全的，只有引用计数是
- 避免循环引用（会导致内存泄漏）

---

## 算法

### 快速排序

基于 `_cmp` 方法的快速排序实现。

**定义:**
```c
QSORT(ElementType)
```

**API:**
```c
void Type_qsort(Type_t *arr, int n);
```

**示例:**
```c
#include "sort.h"
#include "basic_types.h"

// basic_types.h 已预定义: QSORT(int32), QSORT(int64) 等

int main() {
    int32_t arr[] = {5, 2, 8, 1, 9, 3};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    int32_qsort(arr, n);
    
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);  // 1 2 3 5 8 9
    }
    return 0;
}
```

**自定义类型的排序:**
```c
typedef struct {
    int id;
    char name[32];
} person_t;

// 必须定义比较方法
int person_cmp(person_t *a, person_t *b) {
    return int32_cmp(&a->id, &b->id);
}

// 然后在 .c 文件中使用宏
QSORT(person)

void sort_people(person_t *people, int n) {
    person_qsort(people, n);
}
```

---

### 字符串工具

**str.h 提供的工具函数:**

```c
// 字符串处理
char *str_strip(char *str);                    // 去除首尾空白
char **str_split(char *str, char delim);       // 分割字符串
void destroy_str_list(char **list);            // 释放str_split结果

// 字符串构建器
typedef struct {
    char *buf;
    int size;
    int cap;
} str_builder_t;

str_builder_t str_builder_create();
void str_builder_append(str_builder_t *sb, char *format, ...);
void str_builder_append_char(str_builder_t *sb, char c);
str_builder_t str_builder_move(str_builder_t *sb);
void str_builder_destroy(str_builder_t *sb);

// 文件操作
char *fgetline(FILE *fp);  // 读取一行（自动分配内存）
int fpeek(FILE *fp);       // 偷看下一个字符
```

**示例:**
```c
#include "str.h"
#include <stdio.h>

int main() {
    // 字符串构建
    str_builder_t sb = str_builder_create();
    str_builder_append(&sb, "Hello, %s!", "World");
    str_builder_append_char(&sb, '\n');
    printf("%s", sb.buf);
    str_builder_destroy(&sb);
    
    // 分割字符串
    char text[] = "apple,banana,orange";
    char **parts = str_split(text, ',');
    for (int i = 0; parts[i] != NULL; i++) {
        printf("%s\n", parts[i]);
    }
    destroy_str_list(parts);
    
    return 0;
}
```

---

## 工具宏

### NEW / DELETE

```c
#include "utils.h"

// 在堆上创建对象
type_t *ptr = NEW(type, constructor_args);

// 销毁并释放
DELETE(type, ptr);
```

**示例:**
```c
VECTOR(int_vec, int32)

int_vec_t *create_vec_on_heap() {
    return NEW(int_vec);  // 调用 int_vec_create()
}

void destroy_vec_on_heap(int_vec_t *v) {
    DELETE(int_vec, v);  // 调用 int_vec_destroy() 然后 free
}
```

### FOREACH

```c
FOREACH(iter, ContainerPrefix, container_ptr) {
    // 使用 iter
}
```

**示例:**
```c
LIST(int_list, int32)

void print_list(int_list_t *lst) {
    FOREACH(it, int_list, lst) {
        printf("%d ", it->val);
    }
}
```

---

## 常见陷阱

### 1. 迭代器失效

```c
VECTOR(int_vec, int32)
int_vec_t vec = int_vec_create();
int_vec_push_back(&vec, 1);

int32_t *it = int_vec_begin(&vec);
int_vec_push_back(&vec, 2);  // 可能触发realloc

// ❌ it 可能已失效！
printf("%d\n", *it);  // 未定义行为
```

**解决方案:** 操作后重新获取迭代器，或使用索引。

### 2. 未定义基础方法

```c
// ❌ 错误：没有定义 my_type_cmp
typedef struct { int x; } my_type_t;
TREE_MAP(my_tree, my_type, int32)  // 编译/链接错误

// ✅ 正确：先定义比较方法
typedef struct { int x; } my_type_t;
int my_type_cmp(my_type_t *a, my_type_t *b) { return int32_cmp(&a->x, &b->x); }
TREE_MAP(my_tree, my_type, int32)
```

### 3. 重复释放

```c
int_list_t lst1 = int_list_create();
int_list_push_back(&lst1, 1);

int_list_t lst2 = lst1;  // 只是复制了结构体，不是深拷贝
int_list_destroy(&lst1);
int_list_destroy(&lst2);  // ❌ 重复释放！
```

**解决方案:** 使用 `move` 或 `copy`。

### 4. Arc不是线程安全的

```c
ARC(int_arc, int32)
int_arc_t arc = int_arc_create(0);

// Thread 1
(*int_arc_get(&arc))++;  // ❌ 数据竞争！

// Thread 2  
(*int_arc_get(&arc))++;  // ❌ 数据竞争！
```

**解决方案:** Arc只保证引用计数的线程安全，数据访问仍需同步。

### 5. Box的copy陷阱

```c
BOX(int_box, int32)
int_box_t box = int_box_create(42);
int_box_t box2 = int_box_copy(&box);  // ❌ 链接错误！
```

这是设计如此，Box设计为不可复制。

### 6. 忘记销毁

```c
void leaky_function() {
    int_vec_t vec = int_vec_create();
    // ...使用vec
    // ❌ 忘记调用 int_vec_destroy(&vec);
}
```

**解决方案:** 使用RAII模式或仔细检查代码路径。

### 7. 错误使用end()进行访问

```c
// ❌ Tree Map的end()返回NULL，不能解引用
int_tree_iter_t it = int_tree_end(&tree);
printf("%d", it->value);  // 崩溃！

// ✅ 正确用法：end()只是遍历的终止条件
for (it = int_tree_begin(&tree); it != int_tree_end(&tree); ...)
```

---

## 设计模式

### 模式1: RAII资源管理

```c
typedef struct {
    int_vec_t data;
    int_list_t history;
} document_t;

void document_init(document_t *doc) {
    doc->data = int_vec_create();
    doc->history = int_list_create();
}

void document_destroy(document_t *doc) {
    int_vec_destroy(&doc->data);
    int_list_destroy(&doc->history);
}

document_t document_create() {
    document_t doc;
    document_init(&doc);
    return doc;
}

// 使用
document_t doc = document_create();
// ... 使用 doc ...
document_destroy(&doc);
```

### 模式2: 自定义类型的完整实现

```c
// person.h
typedef struct {
    int id;
    char *name;
} person_t;

// 必需方法
void person_init(person_t *p, int id, const char *name);
person_t person_create(int id, const char *name);
void person_destroy(person_t *p);
person_t person_copy(person_t *p);
int person_cmp(person_t *a, person_t *b);
uint64_t person_hash(person_t *p);
bool person_eq(person_t *a, person_t *b);

// person.c
void person_init(person_t *p, int id, const char *name) {
    p->id = id;
    p->name = strdup(name);
}

person_t person_create(int id, const char *name) {
    person_t p;
    person_init(&p, id, name);
    return p;
}

void person_destroy(person_t *p) {
    free(p->name);
    p->name = NULL;
}

person_t person_copy(person_t *p) {
    return person_create(p->id, p->name);
}

int person_cmp(person_t *a, person_t *b) {
    return int32_cmp(&a->id, &b->id);
}

uint64_t person_hash(person_t *p) {
    return mmhash(&p->id, sizeof(p->id), 0);
}

bool person_eq(person_t *a, person_t *b) {
    return a->id == b->id;
}

// 使用
#include "vec.h"
#include "hash_table.h"

VECTOR(person_vec, person)
HASH_TABLE(person_ht, int32, person)
```

### 模式3: 嵌套容器

```c
// 类型别名
VECTOR(int_vec, int32)
VECTOR(int_vec_vec, int_vec)

// 二维动态数组
int_vec_vec_t matrix = int_vec_vec_create();

// 添加行
for (int i = 0; i < 10; i++) {
    int_vec_t row = int_vec_create();
    for (int j = 0; j < 10; j++) {
        int_vec_push_back(&row, i * j);
    }
    int_vec_vec_push_back(&matrix, row);
}

// 访问
*int_vec_ref(int_vec_vec_ref(&matrix, 5), 3);  // matrix[5][3]

// 销毁（需要逐层销毁）
for (int_vec_t *it = int_vec_vec_begin(&matrix);
     it != int_vec_vec_end(&matrix);
     it++) {
    int_vec_destroy(it);
}
int_vec_vec_destroy(&matrix);
```

### 模式4: 智能指针与容器结合

```c
BOX(resource_box, resource_t)
VECTOR(resource_box_vec, resource_box)

// 创建资源列表
resource_box_vec_t resources = resource_box_vec_create();

for (int i = 0; i < 100; i++) {
    resource_t r = resource_create(i);
    resource_box_t box = resource_box_create(r);
    resource_box_vec_push_back(&resources, box);
}

// 使用：无需手动释放每个资源
// vector销毁时会自动调用每个box的destroy
resource_box_vec_destroy(&resources);
```

### 模式5: 惰性求值与缓存

```c
HASH_TABLE(cache, str_t, int_vec)

typedef struct {
    cache_t cache;
} app_t;

int_vec_t *get_data(app_t *app, const char *key) {
    str_t key_str = str_create(key);
    int_vec_t *cached = cache_get(&app->cache, key_str);
    
    if (cached != NULL) {
        str_destroy(&key_str);
        return cached;
    }
    
    // 计算并缓存
    int_vec_t data = compute_expensive(key);
    cache_insert(&app->cache, key_str, data);
    return cache_get(&app->cache, key_str);
}
```

### 模式6: 迭代器适配器模式

```c
// 封装常用遍历模式
#define FILTER_FOREACH(it, container, condition) \
    for (typeof((container)->vhead) it = (container)->vhead->next; \
         it != (container)->vtail; it = it->next) \
    if (condition)

// 使用示例
FILTER_FOREACH(it, &lst, it->val > 10) {
    printf("%d ", it->val);  // 只打印大于10的元素
}
```

---

## 完整示例程序

```c
#include <stdio.h>
#include <stdlib.h>

#include "basic_types.h"
#include "vec.h"
#include "list.h"
#include "hash_table.h"
#include "pqueue.h"
#include "sort.h"

// 定义容器
VECTOR(int_vec, int32)
LIST(int_list, int32)
HASH_TABLE(int_int_map, int32, int32)
PQUEUE(int_pq, int32)

int main() {
    printf("=== ALGDS Demo ===\n\n");
    
    // Vector示例
    printf("[Vector]\n");
    int_vec_t vec = int_vec_create();
    for (int i = 0; i < 5; i++) {
        int_vec_push_back(&vec, i * i);
    }
    for (int32_t *it = int_vec_begin(&vec); it != int_vec_end(&vec); it++) {
        printf("%d ", *it);
    }
    printf("\n\n");
    int_vec_destroy(&vec);
    
    // List示例
    printf("[List]\n");
    int_list_t lst = int_list_create();
    int_list_push_back(&lst, 1);
    int_list_push_back(&lst, 2);
    int_list_push_front(&lst, 0);
    for (int_list_iter_t it = int_list_begin(&lst); 
         it != int_list_end(&lst); 
         it = int_list_next(it)) {
        printf("%d ", it->val);
    }
    printf("\n\n");
    int_list_destroy(&lst);
    
    // Hash Table示例
    printf("[Hash Table]\n");
    int_int_map_t ht = int_int_map_create();
    for (int i = 1; i <= 5; i++) {
        int_int_map_insert(&ht, i, i * 10);
    }
    for (int_int_map_iter_t it = int_int_map_begin(&ht);
         it != int_int_map_end(&ht);
         it = int_int_map_next(&ht, it)) {
        printf("%d -> %d\n", it->key, it->val);
    }
    int_int_map_destroy(&ht);
    printf("\n");
    
    // Priority Queue示例
    printf("[Priority Queue]\n");
    int_pq_t pq = int_pq_create();
    int_pq_push(&pq, 5);
    int_pq_push(&pq, 1);
    int_pq_push(&pq, 10);
    int_pq_push(&pq, 3);
    while (int_pq_top(&pq) != NULL) {
        printf("%d ", *int_pq_top(&pq));
        int_pq_pop(&pq);
    }
    printf("\n\n");
    int_pq_destroy(&pq);
    
    // Sort示例
    printf("[Sort]\n");
    int32_t arr[] = {5, 2, 8, 1, 9, 3};
    int n = sizeof(arr) / sizeof(arr[0]);
    int32_qsort(arr, n);
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    return 0;
}
```
