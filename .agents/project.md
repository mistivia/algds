# ALGDS - 算法与数据结构库编码规范

## 项目概述

这是一个使用宏模板系统提供泛型数据结构和算法的 C 语言库。它在纯 C 中实现了类似 C++ STL 的容器。

## 编程范式

### 1. 基于宏的泛型编程（C 语言中的模板）

该库使用 C 预处理器宏来模拟 C++ 模板：

```c
// 通过宏实例化数据结构
#define DATA_STRUCTURE(A, T) \
    typedef struct { ... } A##_t; \
    void A##_init(A##_t *self); \
    ...

// 用法：实例化一个 int32 类型的向量
VECTOR(int32_vec, int32)
```

**核心原则：**
- `A` = 实例化类型的别名
- `T` = 元素类型（必须有关联的 trait 函数）
- 所有生成的函数使用 `A##_` 前缀以避免命名冲突
- 类型后缀约定：`T##_t`（例如：`int32_t`、`mytype_t`）

### 2. 基于 Trait 的类型系统

类型必须为操作实现 trait 函数：

| Trait 函数 | 用途 |
|-----------|------|
| `T##_eq(a, b)` | 相等比较 |
| `T##_cmp(a, b)` | 顺序比较（返回 -1, 0, 1）|
| `T##_hash(x)` | 哈希表的哈希函数 |
| `T##_copy(x)` | 深拷贝值 |
| `T##_destroy(x)` | 清理值（不释放内存）|
| `T##_delete(x)` | 释放已分配的内存 |
| `T##_new(x)` | 在堆上分配并初始化 |

**基本类型在 [`basic_types.h`](basic_types.h:1)** 中通过 [`BASIC_TYPE`](basic_types.h:18) 宏提供。

### 3. RAII 和所有权语义

**智能指针类型：**
- [`BOX`](box.h:6) - 独占所有权（仅可移动，作用域结束时销毁）
- [`ARC`](arc.h:7) - 共享所有权（原子引用计数）

**标准方法模式：**
- `A##_create()` - 构造函数（按值返回）
- `A##_init(&self)` - 初始化现有实例
- `A##_destroy(&self)` - 析构函数（清理资源）
- `A##_move(&self)` - 转移所有权（使源对象无效）
- `A##_copy(&self)` - 克隆（深拷贝，如适用）

### 4. 迭代器模式

大多数容器提供基于迭代器的访问：

```c
// 标准迭代器接口
A##_iter_t A##_begin(A##_t *self);      // 第一个元素
A##_iter_t A##_end(A##_t *self);        // 末尾之后（NULL 或哨兵）
A##_iter_t A##_next(A##_t *self, iter); // 前进
A##_iter_t A##_prev(iter);              // 后退
T##_t* A##_get(A##_t *self, iter);      // 解引用
```

**迭代器类型：**
- 链表：指向节点的指针（`A##_node_*`）
- 扁平列表：数组的整数索引
- 向量：指向元素的指针

## 代码风格

### 命名约定

| 元素 | 模式 | 示例 |
|-----|------|------|
| 类型 | `snake_case_t` | `int32_t`、`str_t` |
| 函数 | `prefix_snake_case` | `int32_list_push_back` |
| 宏 | `SCREAMING_SNAKE_CASE` | `VECTOR`、`FLAT_LIST` |
| 私有类型 | `A##__typename_` | `int32_list_node__t` |
| 私有函数 | `static inline` | `A##_alloc_node` |

### 头文件保护

```c
#ifndef ALGDS_<NAME>_H_
#define ALGDS_<NAME>_H_
// ...
#endif
```

### 函数属性

- `__attribute__((weak))` - 允许用户覆盖默认实现
- `static inline` - 用于内部辅助函数

### 内存管理

1. **分配：** 直接使用 `malloc()`、`realloc()`
2. **释放：** 始终与适当的清理配对
3. **置空：** 释放后将指针设为 NULL，防止释放后使用
4. **哨兵：** 在列表中使用哨兵节点简化逻辑

```c
// destroy 模式
void A##_destroy(A##_t *self) {
    // 清理元素
    for (每个元素) {
        T##_destroy(&element);
    }
    free(self->buffer);
    self->buffer = NULL;  // 防止悬空指针
}
```

### 错误处理

- 指针失败返回 `NULL`
- 成功/失败操作返回布尔值
- 在测试中使用断言，不在库代码中使用

## 架构模式

### 1. 容器结构

```c
typedef struct {
    // 内部存储
    storage_type data;
    
    // 元数据
    size_t len;
    int cap;
    
    // 类型特定（可选）
    int free_head;  // 用于 flat_list 的空闲列表
} A##_t;
```

### 2. 扁平列表（基于数组的链表）

使用数组索引而非指针的专用列表：
- 节点存储在连续数组（向量）中
- `prev`/`next` 是整数索引，不是指针
- 空闲列表用于回收删除的节点
- 索引 0（头）和 1（尾）处的哨兵节点

### 3. 组合优于继承

容器组合较小的组件：
- [`FLAT_LIST`](flat_list.h:8) 内部使用 [`VECTOR`](vec.h:7)
- [`PQUEUE`](pqueue.h:6) 使用 [`VECTOR`](vec.h:7) 作为堆存储
- [`HASH_TABLE`](hash_table.h:7) 包装通用 [`HashTable`](hash_table.h:98)

### 4. 弱符号用于自定义

声明为 `__attribute__((weak))` 的函数可以被覆盖：

```c
// 库提供默认值
void A##_init(A##_t *self) __attribute__((weak));

// 用户可以提供自定义实现
void mytype_init(mytype_t *self) { /* 自定义 */ }
```

## 文件组织

| 文件 | 用途 |
|-----|------|
| [`basic_types.h`](basic_types.h:1) | 基本类型的类型 trait |
| [`vec.h`](vec.h:1) | 动态数组（向量）|
| [`list.h`](list.h:1) | 双向链表（基于指针）|
| [`flat_list.h`](flat_list.h:1) | 双向链表（基于索引）|
| [`pqueue.h`](pqueue.h:1) | 二叉堆优先队列 |
| [`hash_table.h`](hash_table.h:1) | 哈希表（键值对）|
| [`tree_map.h`](tree_map.h:1) | 红黑树映射 |
| [`box.h`](box.h:1) | 独占指针 |
| [`arc.h`](arc.h:1) | 引用计数共享指针 |
| [`sort.h`](sort.h:1) | 通用快速排序 |
| [`str.h`](str.h:1) | 字符串工具 |
| [`utils.h`](utils.h:1) | 工具函数：MurmurHash, mallocz|

## 测试约定

测试位于 `tests/` 目录，命名格式为 `test_<模块>.c`：

```c
#include "header.h"
#include "basic_types.h"
#include <assert.h>
#include <stdio.h>

// 实例化类型
DATA_STRUCTURE(my_type, int32)
// DATA_STRUCTURE(my_type, my_base_type)

int main(void) {
    printf("[TEST] module_name\n");
    
    // 使用断言的测试代码
    assert(condition);
    
    printf("[PASS] module_name\n");
    return 0;
}
```

## 构建系统

- 使用 [`Makefile`](Makefile:1) 构建
- 测试运行脚本：[`scripts/runall.sh`](scripts/runall.sh:1)
- Clangd 配置：[`.clangd`](.clangd:1)

## 关键设计决策

1. **仅头文件模板：** 所有实现都在头文件中，以便宏展开
2. **显式类型后缀：** `T##_t` 模式确保一致性
3. **弱符号：** 无需修改即可启用自定义
4. **迭代器抽象：** 跨容器类型的统一接口
6. **零成本抽象：** 宏展开为直接的 C 代码
