#ifndef ALGDS_UTILS_H_
#define ALGDS_UTILS_H_

#define NEW(Type, ...) ({ \
    Type *_ptr = (Type *)malloc(sizeof(Type)); \
    if (_ptr != NULL) { \
        *_ptr = Type##_create(__VA_ARGS__); \
    } \
    _ptr; \
})

#define DELETE(Type, _x) do { \
    Type##_destroy(_x); \
    free(_x); \
} while (0);

#define FOREACH(_iter, A, _a) \
    for (A##Iter _iter = A##_begin(_a); _iter != A##_end(_a); _iter = A##_next(_a, _iter))

#endif