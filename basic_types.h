#ifndef ALGDS_BAISC_TRAITS_H_
#define ALGDS_BAISC_TRAITS_H_

#include "mmhash.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef bool bool_t;
typedef char char_t;
typedef float real32_t;
typedef double real64_t;
typedef void *ptr_t;

// basic traits
#define BASIC_TYPE(T) \
    static inline bool T##_eq(T##_t *lhs, T##_t *rhs) { \
        return *lhs == *rhs; \
    } \
    static inline int T##_cmp(T##_t *lhs, T##_t *rhs) { \
        if (*lhs == *rhs) return 0; \
        if (*lhs < *rhs) return -1; \
        return 1; \
    } \
    static inline uint64_t T##_hash(T##_t *x) { \
        return mmhash(x, sizeof(T##_t), 0); \
    } \
    static inline void T##_destroy(T##_t *x) {} \
    static inline T##_t T##_copy(T##_t *x) { \
        return *x; \
    }

BASIC_TYPE(int32)
BASIC_TYPE(int64)
BASIC_TYPE(bool);
BASIC_TYPE(char);
BASIC_TYPE(uint32);
BASIC_TYPE(uint64);
BASIC_TYPE(real32);
BASIC_TYPE(real64);

typedef const char *str_t;

static inline bool
str_eq(str_t* a, str_t *b)
{
    return strcmp(*a, *b) == 0;
}

static inline int
str_cmp(str_t* a, str_t *b)
{
    return strcmp(*a, *b);
}

static inline str_t
str_copy(str_t *a)
{
    return strdup(*a);
}

static inline uint64_t
str_hash(str_t *self)
{
    size_t len = strlen(*self);
    return mmhash(*self, len, 0);
}

static inline void
str_destroy(str_t *self)
{
    free((void*)*self);
}

#endif