#ifndef ALGDS_VEC_H_
#define ALGDS_VEC_H_

#include <stdlib.h>
#include <string.h>

#define VECTOR(A, T) \
    typedef struct { \
        T##_t *buffer; \
        int size; \
        int cap; \
    } A##_t; \
    \
    A##_t A##_create()  __attribute__((weak)); \
    void A##_push_back(A##_t *self, T##_t elem)  __attribute__((weak)); \
    void A##_insert_before(A##_t *self, int n, T##_t elem)  __attribute__((weak)); \
    void A##_pop(A##_t *self)  __attribute__((weak)); \
    bool A##_empty(A##_t *self)  __attribute__((weak)); \
    void A##_remove(A##_t *self, size_t n)  __attribute__((weak)); \
    size_t A##_len(A##_t *self)  __attribute__((weak)); \
    T##_t* A##_begin(A##_t *self)  __attribute__((weak)); \
    T##_t* A##_last(A##_t *self)  __attribute__((weak)); \
    T##_t* A##_end(A##_t *self)  __attribute__((weak)); \
    T##_t* A##_ref(A##_t *self, size_t n)  __attribute__((weak)); \
    void A##_swap(A##_t *self, int i, int j)  __attribute__((weak)); \
    A##_t A##_move(A##_t *self)  __attribute__((weak)); \
    void A##_destroy(A##_t *self)  __attribute__((weak)); \
    A##_t A##_copy(A##_t *self)  __attribute__((weak));\
    \
    A##_t A##_create() { \
        A##_t self = {0}; \
        self.buffer = (T##_t *)malloc(sizeof(T##_t) * 16); \
        self.cap = 16; \
        self.size = 0; \
        return self; \
    } \
    void A##_push_back(A##_t *self, T##_t elem) { \
        if (self->size + 1 > self->cap) { \
            self->buffer = realloc(self->buffer, sizeof(T##_t) * self->cap * 2); \
            self->cap *= 2; \
        } \
        self->buffer[self->size] = elem; \
        self->size += 1; \
    } \
    void A##_insert_before(A##_t *self, int n, T##_t elem) { \
        if (n < 0 || n > self->size) { \
            return; \
        } \
        if (self->size + 1 > self->cap) { \
            self->buffer = malloc(sizeof(T##_t) * self->cap * 2); \
            self->cap *= 2; \
        } \
        if (n != self->size) { \
            memmove(self->buffer + n + 1, \
                    self->buffer + n, \
                    sizeof(T##_t) * (self->size - n)); \
            self->buffer[n] = elem; \
            self->size += 1; \
        } \
    } \
    void A##_pop(A##_t *self) { \
        self->size -= 1; \
        T##_destroy(self->buffer + self->size - 1); \
    } \
    void A##_remove(A##_t *self, size_t n) { \
        if (n < 0 || n >= self->size) return; \
        T##_destroy(self->buffer + n); \
        memmove(self->buffer + n, \
                self->buffer + n + 1, \
                sizeof(T##_t) * self->size - n - 1); \
        self->size -= 1; \
    } \
    T##_t* A##_begin(A##_t *self) { return self->buffer; } \
    bool A##_empty(A##_t *self) { return self->size == 0; } \
    T##_t* A##_end(A##_t *self) { return self->buffer + self->size; } \
    T##_t* A##_last(A##_t *self) { return self->buffer + self->size - 1; } \
    T##_t* A##_ref(A##_t *self, size_t n) { return self->buffer + n; } \
    void A##_swap(A##_t *self, int i, int j) { \
        T##_t buf = {0}; \
        memcpy(&buf, self->buffer + i, sizeof(T##_t)); \
        memcpy(self->buffer + i, self->buffer + j, sizeof(T##_t)); \
        memcpy(self->buffer + j, &buf, sizeof(T##_t)); \
    } \
    A##_t A##_move(A##_t *self) { \
        A##_t dup = *self; \
        self->buffer = NULL; \
        self->size = 0; \
        self->cap = 0; \
        return dup; \
    } \
    size_t A##_len(A##_t *self) { return self->size; } \
    void A##_destroy(A##_t *self) {\
        for (int i = 0; i < self->size; i++) { \
            T##_destroy(&self->buffer[i]); \
        } \
        free(self->buffer); \
    } \
    A##_t A##_copy(A##_t *self) {\
        A##_t ret = A##_create(); \
        for (int i = 0; i < self->size; i++) { \
            A##_push_back(&ret, T##_copy(&self->buffer[i])); \
        } \
        return ret; \
    }

#endif
