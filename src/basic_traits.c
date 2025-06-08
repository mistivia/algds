#include "basic_traits.h"

#include <string.h>

#include "mmhash.h"

#define BASIC_TRAITS_IMPL(T) \
    bool T##_eq(T* lhs, T* rhs) { \
        return *lhs == *rhs; \
    } \
    int T##_cmp(T* lhs, T* rhs) { \
        if (*lhs == *rhs) return 0; \
        if (*lhs < *rhs) return -1; \
        return 1; \
    } \
    uint64_t T##_hash(T* x) { \
        return mmhash(x, sizeof(T), 0); \
    }

BASIC_TRAITS_IMPL(Char);
BASIC_TRAITS_IMPL(Bool);
BASIC_TRAITS_IMPL(Int);
BASIC_TRAITS_IMPL(Long);
BASIC_TRAITS_IMPL(UInt);
BASIC_TRAITS_IMPL(ULong);
BASIC_TRAITS_IMPL(Double);
BASIC_TRAITS_IMPL(Float);

bool String_eq(String* lhs, String *rhs) {
    return strcmp(*lhs, *rhs) == 0;
}

int String_cmp(String* lhs, String *rhs) {
    return strcmp(*lhs, *rhs);
}

ULong String_hash(String *x) {
    size_t len = strlen(*x);
    return mmhash(*x, len, 0);
}


