# Type-safe Generic Algorithms and Data Structures for C

- Generic red-black tree
- Generic vector
- Generic linked-list
- Generic array-based linked list (AList)
- Generic priority queue
- Generic Hash Table
- Generic quick sort
- Augmented red-black tree
- Murmur Hash
- String utilities

## Documentation

- [AGENTS.md](AGENTS.md): Architectural overview and instructions for AI coding agents.
- [docs/usage.md](docs/usage.md): Detailed guide on how to use the generic macro system and implement traits.

## Build & Install

    make
    make install

## Run Tests

    make test

## Quickstart

This library uses C macros to provide generic data structures. For any custom type `T`, you need to define "traits" (functions like `T_eq`, `T_cmp`, `T_hash`, `T_show`) and use declaration/implementation macros.

**Important:** Always use the `*_DEF_AS` and `*_IMPL_AS` variants of the macros.

### Example: Linked List

If you have a custom type `FancyThing`, and you want a linked list of them:

1. In your header file: `LIST_DEF_AS(FancyThing, FancyList)`
2. In your source file: `LIST_IMPL_AS(FancyThing, FancyList)`

This generates functions like `FancyList_create`, `FancyList_push_back`, `FancyList_free`, etc.

### Example: Hash Table

If you want a hash table with `FancyThing` as key and `Int` as value:

1. Implement the required traits:
   - `uint64_t FancyThing_hash(FancyThing self)`
   - `bool FancyThing_eq(FancyThing a, FancyThing b)`
2. In your header file: `HASH_TABLE_DEF_AS(FancyThing, Int, FancyTable)`
3. In your source file: `HASH_TABLE_IMPL_AS(FancyThing, Int, FancyTable)`

### Important: T_show Trait
Containers like `Vector` and `AList` require the `T_show` trait to be defined at the time of instantiation (`*_IMPL_AS`), as their internal implementations depend on it.

## Memory Management

The `_free` functions provided by the containers (e.g., `FancyList_free`) only free the container's internal management structures (nodes, buffers). They **do not** free the elements stored inside if they are pointers. The user is responsible for the lifecycle of the stored elements.
