#ifndef ALGDS_VEC_H_
#define ALGDS_VEC_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "type_alias.h"

#define DEF_VECTOR(T, A) \
    typedef struct { \
        T* buffer; \
        int size; \
        int cap; \
    } A; \
    \
    void A##_init(A *self); \
    A A##_create(); \
    void A##_push_back(A *self, T elem); \
    void A##_insert_before(A *self, int n, T elem); \
    void A##_pop(A *self); \
    bool A##_empty(A *self); \
    void A##_remove(A *self, size_t n); \
    size_t A##_len(A *self); \
    T* A##_begin(A *self); \
    T* A##_last(A *self); \
    T* A##_end(A *self); \
    T* A##_ref(A *self, size_t n); \
    void A##_swap(A *self, int i, int j); \
    A A##_move(A *self); \
    void A##_show(A self, FILE* fp); \
    void A##_free(A *self);

#define VECTOR_DEF(T) DEF_VECTOR(T, T##Vector)

#define IMPL_VECTOR(T, A) \
    void A##_init(A *self) { \
        self->buffer = (T*)malloc(sizeof(T) * 16); \
        self->cap = 16; \
        self->size = 0; \
    } \
    A A##_create() { \
        A self; \
        A##_init(&self); \
        return self; \
    } \
    void A##_push_back(A *self, T elem) { \
        if (self->size + 1 > self->cap) { \
            self->buffer = realloc(self->buffer, sizeof(T) * self->cap * 2); \
            self->cap *= 2; \
        } \
        self->buffer[self->size] = elem; \
        self->size += 1; \
    } \
    void A##_insert_before(A *self, int n, T elem) { \
        if (n < 0 || n > self->size) { \
            return; \
        } \
        if (self->size + 1 > self->cap) { \
            self->buffer = malloc(sizeof(T) * self->cap * 2); \
            self->cap *= 2; \
        } \
        if (n != self->size) { \
            memmove(self->buffer + n + 1, \
                    self->buffer + n, \
                    sizeof(T) * (self->size - n)); \
            self->buffer[n] = elem; \
            self->size += 1; \
        } \
    } \
    void A##_pop(A *self) { \
        self->size -= 1; \
    } \
    void A##_remove(A *self, size_t n) { \
        if (n < 0 || n >= self->size) return; \
        memmove(self->buffer + n, \
                self->buffer + n + 1, \
                sizeof(T) * self->size - n - 1); \
        self->size -= 1; \
    } \
    T* A##_begin(A *self) { return self->buffer; } \
    bool A##_empty(A *self) { return self->size == 0; } \
    T* A##_end(A *self) { return self->buffer + self->size; } \
    T* A##_last(A *self) { return self->buffer + self->size - 1; } \
    T* A##_ref(A *self, size_t n) { return self->buffer + n; } \
    void A##_swap(A *self, int i, int j) { \
        T buf; \
        memcpy(&buf, self->buffer + i, sizeof(T)); \
        memcpy(self->buffer + i, self->buffer + j, sizeof(T)); \
        memcpy(self->buffer + j, &buf, sizeof(T)); \
    } \
    A A##_move(A *self) { \
        A dup = *self; \
        self->buffer = NULL; \
        self->size = 0; \
        self->cap = 0; \
        return dup; \
    } \
    void A##_show(A self, FILE* fp) { \
        fprintf(fp, "["); \
        for (int i = 0; i < self.size-1; i++) { \
            T##_show(self.buffer[i], fp); \
            fprintf(fp, ", "); \
        } \
        if (self.size > 0) { \
            T##_show(self.buffer[self.size - 1], fp); \
        } \
        fprintf(fp, "]"); \
    } \
    size_t A##_len(A *self) { return self->size; } \
    void A##_free(A *self) { free(self->buffer); }

#define VECTOR_IMPL(T) IMPL_VECTOR(T, T##Vector)

VECTOR_DEF(Int);
VECTOR_DEF(Bool);
VECTOR_DEF(Long);
VECTOR_DEF(Char);
VECTOR_DEF(UInt);
VECTOR_DEF(ULong);
VECTOR_DEF(Double);
VECTOR_DEF(Float);
VECTOR_DEF(String);
VECTOR_DEF(VoidPtr);

#endif
