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

#endif