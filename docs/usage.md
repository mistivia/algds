# AI Coding Agent Usage Guide for algds

This guide explains how an AI coding agent should interact with and use the `algds` library to implement data structures and algorithms.

## Core Principle: Macro-Based Generics

`algds` uses C macros to simulate generics. For any type `T` you want to use with a container (Vector, List, etc.), you must follow a two-step process: **Declaration** and **Implementation**.

### Important Notes
- **Macro Deprecation:** The older `*_DEF(T)` and `*_IMPL(T)` macros are **deprecated**. Always use the `*_DEF_AS(T, A)` and `*_IMPL_AS(T, A)` variants to specify a clear alias for the resulting type.
- **Memory Management:** The `_free` function ONLY frees the container's internal buffers or node structures. It **does not** free the objects stored inside if they are pointers.
- **AList:** This is an **Array-based Linked List**, not a standard "Array List" (Vector). It provides linked list functionality (O(1) insertion/deletion at any iterator) while being stored in a contiguous array for better performance.

- `list.h`中的list在迭代的时候要用这样的模式，而**不是**判断是否为NULL：
```
while (it != XXXList_end(lst)) {
    // ...
    it = XXXList_next(it);
}
```


### 1. Identify or Define Traits
Every type `T` used in a container MUST have the following "traits" (functions) defined if the container's macros expect them:
- `Bool T_eq(T lhs, T rhs)`: Equality check.
- `Int T_cmp(T lhs, T rhs)`: Comparison (returns <0, 0, or >0).
- `uint64_t T_hash(T x)`: Hashing function.
- `void T_show(T x, FILE* fp)`: Debug printing.

**Dependency Note:** Both `Vector` and `AList` macros include a `_show` function implementation that directly calls `T_show`. If you instantiate these containers for a type `T` using the `_IMPL_AS` macro, you **must** provide `T_show` or you will encounter compilation errors, even if you do not explicitly call the container's `_show` function.

Common types (`Int`, `String`, `Bool`, etc.) are already defined in `basic_traits.h`.

### 2. Using a Container with a Custom Type

If you have a struct `Person` and want a `Vector` of them:

#### In the Header (`.h`)
```c
#include "vec.h"

typedef struct {
    char* name;
    int age;
} Person;

// Declare the Vector type 'PersonVector'
DEF_VECTOR(Person, PersonVector)
```

#### In the Source (`.c`)
```c
#include "person.h"

// Define traits for Person
Bool Person_eq(Person a, Person b) { ... }
Int Person_cmp(Person a, Person b) { ... }
uint64_t Person_hash(Person p) { ... }
void Person_show(Person p, FILE* fp) { ... }

// Implement the Vector logic for PersonVector
IMPL_VECTOR(Person, PersonVector)
```

## Available Macros

| Data Structure | Header | Declaration Macro | Implementation Macro |
| :--- | :--- | :--- | :--- |
| Vector | `vec.h` | `DEF_VECTOR(T, A)` | `IMPL_VECTOR(T, A)` |
| Linked List | `list.h` | `DEF_LIST(T, A)` | `IMPL_LIST(T, A)` |
| AList | `alist.h` | `DEF_ALIST(T, A)` | `IMPL_ALIST(T, A)` |
| Hash Table | `hash_table.h` | `DEF_HASH_TABLE(K, V, A)` | `IMPL_HASH_TABLE(K, V, A)` |
| Priority Queue | `pqueue.h` | `PQUEUE_DEF_AS(T, A)` | `IMPL_PQUEUE(T, A)` |
| Tree Map | `tree_map.h` | `DEF_TREE_MAP(K, V, A)` | `IMPL_TREE_MAP(K, V, A)` |

## Best Practices for Agents
1. **Check for existing instantiations:** Before implementing `IMPL_VECTOR(Int, IntVector)`, check `vec.c` to see if it's already there.
2. **Follow Naming Conventions:** Use `TypeAlias` (e.g., `IntVector`) for the second argument of the `_AS` macros.
3. **Trait Consistency:** Ensure trait names exactly match `T_traitname`.
4. **Memory Management:** Always call the `_free` function (e.g., `IntVector_free(&vec)`) when a container is no longer needed.
