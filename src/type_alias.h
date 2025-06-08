#ifndef ALGDS_TYPE_ALIAS_H_
#define ALGDS_TYPE_ALIAS_H_

#include <stdint.h>
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
typedef const void *VoidPtr;

typedef uint64_t (*VoidHashFn)(void*);
typedef bool (*VoidEqFn)(void*, void*);
typedef int (*VoidCmpFn)(void*, void*);

#endif
