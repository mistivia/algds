# GEMINI.md - algds Project Context

This project, `algds`, is a collection of type-safe, generic data structures and algorithms implemented in C. It uses a macro-based system to provide generic behavior similar to templates in C++ or generics in Rust/Haskell.

## Project Overview

- **Purpose:** Provide high-performance, type-safe, and generic data structures for C.
- **Main Technologies:** C (C99+), GNU Make.
- **Architecture:** 
    - **Macro Generics:** Each data structure is defined via a set of macros (e.g., `DEF_VECTOR` and `IMPL_VECTOR`).
    - **Traits Pattern:** The library relies on "traits" (functions like `T_eq`, `T_cmp`, `T_hash`, `T_show`) that must be defined for any type used in the generic containers.
    - **Pre-instantiation:** Common types like `Int`, `String`, `Bool`, etc., are pre-instantiated in the library's `.c` files.

### Key Data Structures
- **Vector:** `vec.h` / `vec.c`
- **Linked List:** `list.h` / `list.c`
- **AList (Array-based Linked List):** `alist.h` / `alist.c`. Provides the same functionality as `list` but implemented on top of a `Vector` for better memory efficiency and locality.
- **Hash Table:** `hash_table.h` / `hash_table.c`
- **Priority Queue:** `pqueue.c` / `pqueue.h`
- **Red-Black Tree (TreeMap):** `tree_map.c` / `tree_map.h`
- **Augmented RB-Tree:** `tree_map.c` (supports custom augmentation logic)

### Algorithms
- **Sort:** `sort.h` / `sort.c` (Generic quicksort)
- **Hash:** `mmhash.h` / `mmhash.c` (MurmurHash implementation)
- **String Utilities:** `str.h` / `str.c`

## Building and Running

### Commands
- **Build the library:** `make` (Default mode is `debug`).
- **Release build:** `mode=release make`.
- **Run tests:** `make test`.
- **Install:** `make install` (Requires sudo, installs headers to `/usr/local/include/algds`, library to `/usr/local/lib`, and documentation to `/usr/local/share/algds`).
- **Clean:** `make clean`.

### Dependencies
- A C compiler (defaults to `gcc`).
- Standard C library.

## Development Conventions

### Generic Implementation Pattern
To use a data structure with a custom type `T`:
1.  **Define Traits:** Implement `T_eq`, `T_cmp`, `T_hash`, and `T_show` if needed.
    - **Note:** Both `Vector` and `AList` **require** the `T_show` trait to be defined when their implementation macro is used. Failing to define `T_show` will result in a compilation error because the container's internal `_show` function depends on it.
2.  **Declare in Header:** Use `DATA_STRUCTURE_DEF_AS(T, T_Alias)` in your `.h` file.
3.  **Implement in Source:** Use `DATA_STRUCTURE_IMPL_AS(T, T_Alias)` in your `.c` file.

**Note on Macros:** The older `*_DEF` and `*_IMPL` macros are **deprecated**. Always use the `*_AS` variants to ensure explicit naming and avoid collisions.

Example for a List of `MyType`:
```c
// In header
DEF_LIST(MyType, MyList)

// In source
IMPL_LIST(MyType, MyList)
```

### Naming Conventions
- Types are usually CamelCase (e.g., `IntVector`).
- Functions are prefixed with the type alias and an underscore (e.g., `IntVector_push_back`).
- Macros for definition/implementation are uppercase (e.g., `DEF_VECTOR`).

### Post-Task Requirements
After completing a task (adding a feature, fixing a bug, etc.), the agent MUST:
1.  Update `AGENTS.md` if any architectural or structural changes were made.
2.  Update `README.md` if user-facing features or build steps changed.
3.  Update `docs/usage.md` if new data structures or usage patterns were introduced.

### Coding Style
- Hardened compiler flags are used (see `Makefile`).
- **Memory Management:** Memory management is manual.
    - Most structures provide a `_free` function (e.g., `A##_free`).
    - **CRITICAL:** The `_free` function only frees the container's internal management structures (buffers, nodes). It **does not** free the objects stored inside if they are pointers, nor does it free pointers passed as input. The user is responsible for managing the lifecycle of elements stored in the containers.
- Uses `type_alias.h` for consistent type naming (e.g., `Int` instead of `int32_t`).
