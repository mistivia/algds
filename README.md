# Type-safe Generic Algorithms and Data Structures for C

- Generic red-black tree
- Generic vector
- Generic linked-list
- Generic priority queue
- Generic Hash Table
- Generic quick sort
- Augmented red-black tree
- Murmur Hash
- String utilities

## Build & Install

    make
    make install

## Run Tests

    make test

## Quickstart

For example, if you have a custom type `FancyThing`, and you want a linked list of `FancyThing`s. Just add `LIST_DEF_AS(FancyThing, FancyThings)` to the header file, and `LIST_IMPL_AS(FancyThing, FancyThings)` to a source file, and you get a new type `FancyThings`, and many functions for free:

- `FancyThings_create`
- `FancyThings_push_back`
- `FancyThings_push_front`
- `FancyThings_pop_back`
- `FancyThings_pop_front`
- `FancyThings_pop_front`
- ...

And if you want a hash table with `FancyThing` as key, and a integer as value. First, define and implement 2 functions:

- `uint64_t FancyThing_hash(FancyThing self)`
- `bool FancyThing_eq(FancyThing a, FancyThing b)`

These functions just act like how typeclass/trait/concept works in Haskell/Rust/C++. And we provide `mmhash` to help you build `FancyThing_hash`.

Then you can add `HASH_TABLE_DEF_AS(FancyThing, Int, FancyTable)` to the header file, and `HASH_TABLE_IMPL_AS(FancyThing, Int, FancyTable)` to a source file. Then you get a new type `FancyTable` and its functions:

- `FancyTable_create`
- `FancyTable_free`
- `FancyTable_insert`
- `FancyTable_find`
- ...

And other data structures follow similar patterns.
