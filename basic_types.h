#ifndef ALGDS_BAISC_TRAITS_H_
#define ALGDS_BAISC_TRAITS_H_

#include "mmhash.h"

#include <stdbool.h>

typedef bool Bool;
typedef int32_t Int;
typedef int64_t Long;
typedef uint32_t UInt;
typedef uint64_t ULong;
typedef char Char;
typedef float Float;
typedef double Double;
typedef const char *String;
typedef const void *Ptr;

// basic traits
#define BAISC_TYPE(T) \
    static inline bool T##_eq(T lhs, T rhs) { \
        return lhs == rhs; \
    } \
    static inline int T##_cmp(T lhs, T rhs) { \
        if (lhs == rhs) return 0; \
        if (lhs < rhs) return -1; \
        return 1; \
    } \
    static inline uint64_t T##_hash(T x) { \
        return mmhash(&x, sizeof(T), 0); \
    } \
    static inline void T##_destroy(T *x) {} \
    static inline T T##_copy(T *x) { \
        return *x; \
    } \

BAISC_TYPE(Int);
BAISC_TYPE(Bool);
BAISC_TYPE(Long);
BAISC_TYPE(Char);
BAISC_TYPE(UInt);
BAISC_TYPE(ULong);
BAISC_TYPE(Double);
BAISC_TYPE(Float);
BAISC_TYPE(Ptr);


#endif
