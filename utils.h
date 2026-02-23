#ifndef ALGDS_UTILS_H_
#define ALGDS_UTILS_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint64_t mmhash(const void *key, int len, uint64_t seed);

static inline void* mallocz(size_t sz) {
    void *ptr = malloc(sz);
    memset(ptr, 0, sz);
    return ptr;
}

#endif
