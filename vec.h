#ifndef ALGDS_VEC_H_
#define ALGDS_VEC_H_

#include <stdlib.h>
#include <string.h>

#define VECTOR(A, T) \
    typedef struct { \
        T* buffer; \
        int size; \
        int cap; \
    } A; \
    \
    A A##_create()  __attribute__((weak)); \
    void A##_push_back(A *self, T elem)  __attribute__((weak)); \
    void A##_insert_before(A *self, int n, T elem)  __attribute__((weak)); \
    void A##_pop(A *self)  __attribute__((weak)); \
    bool A##_empty(A *self)  __attribute__((weak)); \
    void A##_remove(A *self, size_t n)  __attribute__((weak)); \
    size_t A##_len(A *self)  __attribute__((weak)); \
    T* A##_begin(A *self)  __attribute__((weak)); \
    T* A##_last(A *self)  __attribute__((weak)); \
    T* A##_end(A *self)  __attribute__((weak)); \
    T* A##_ref(A *self, size_t n)  __attribute__((weak)); \
    void A##_swap(A *self, int i, int j)  __attribute__((weak)); \
    A A##_move(A *self)  __attribute__((weak)); \
    void A##_destroy(A *self)  __attribute__((weak)); \
    \
    A A##_create() { \
        A self; \
        self.buffer = (T*)malloc(sizeof(T) * 16); \
        self.cap = 16; \
        self.size = 0; \
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
        T##_destroy(self->buffer + self->size - 1); \
    } \
    void A##_remove(A *self, size_t n) { \
        if (n < 0 || n >= self->size) return; \
        T##_destroy(self->buffer + n); \
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
    size_t A##_len(A *self) { return self->size; } \
    void A##_destroy(A *self) {\
        for (int i = 0; i < self->size; i++) { \
            T##_destroy(&self->buffer[i]); \
        } \
        free(self->buffer); \
    }

#endif
