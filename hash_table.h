#ifndef ALGDS_HASH_TABLE_H_
#define ALGDS_HASH_TABLE_H_

#include <stdbool.h>
#include <stdint.h>

#define HASH_TABLE(A, K, V) \
    typedef struct { \
        K##_t key; \
        V##_t val; \
    } A##_entry_; \
    typedef A##_entry_ *A##_iter_t; \
    typedef struct { \
        HashTable ht; \
    } A##_t; \
    \
    static inline void A##_init(A##_t *self); \
    A##_t A##_create() __attribute__((weak)); \
    void A##_destroy(A##_t *self) __attribute__((weak)); \
    A##_t A##_move(A##_t *self) __attribute__((weak)); \
    bool A##_insert(A##_t *self, K##_t key, V##_t value) __attribute__((weak)); \
    void A##_remove(A##_t *self, A##_iter_t iter) __attribute__((weak)); \
    V##_t *A##_get(A##_t *self, K##_t key) __attribute__((weak)); \
    A##_iter_t A##_find(A##_t *self, K##_t key) __attribute__((weak)); \
    A##_iter_t A##_begin(A##_t *self) __attribute__((weak)); \
    A##_iter_t A##_end(A##_t *self) __attribute__((weak)); \
    A##_iter_t A##_next(A##_t *self, A##_iter_t iter) __attribute__((weak)); \
    size_t A##_len(A##_t *self) __attribute__((weak)); \
    \
    static inline uint64_t A##_hash(void *vk) { \
        K##_t *k = vk; \
        return K##_hash(k); \
    } \
    static inline bool A##_eq(void *vk1, void *vk2) { \
        K##_t *k1 = vk1, *k2 = vk2; \
        return K##_eq(k1, k2); \
    } \
    static inline void A##_init(A##_t *self) { \
        init_hash_table(&self->ht, sizeof(A##_entry_), 16); \
    } \
    A##_t A##_create() { \
        A##_t self; \
        A##_init(&self); \
        return self; \
    } \
    bool A##_insert(A##_t *self, K##_t key, V##_t value) { \
        A##_entry_ entry; \
        entry.key = key; \
        entry.val = value; \
        return hash_table_insert(&self->ht, &entry, A##_hash, A##_eq); \
    } \
    A##_iter_t A##_find(A##_t *self, K##_t key) { \
        return hash_table_find(&self->ht, &key, A##_hash, A##_eq); \
    } \
    V##_t *A##_get(A##_t *self, K##_t key) { \
        A##_entry_ *entry = hash_table_find(&self->ht, &key, A##_hash, A##_eq); \
        if (entry == NULL) return NULL; \
        return &(entry->val); \
    } \
    void A##_remove(A##_t *self, A##_iter_t iter) { \
        K##_destroy(&iter->key); \
        V##_destroy(&iter->val); \
        hash_table_remove(&self->ht, iter); \
    } \
    A##_iter_t A##_begin(A##_t *self) { \
        return hash_table_begin(&self->ht); \
    } \
    A##_iter_t A##_next(A##_t *self, A##_iter_t iter) { \
        return hash_table_next(&self->ht, iter); \
    } \
    A##_iter_t A##_end(A##_t *self) { \
        (void)self; \
        return NULL; \
    } \
    size_t A##_len(A##_t *self) { \
        return (size_t)self->ht.size; \
    } \
    void A##_destroy(A##_t *self) { \
        A##_iter_t it = A##_begin(self); \
        while (it != A##_end(self)) { \
            K##_destroy(&it->key); \
            V##_destroy(&it->val); \
            it = A##_next(self, it); \
        } \
        destroy_hash_table(&self->ht); \
    } \
    A##_t A##_move(A##_t *self) { \
        A##_t dup; \
        dup.ht = self->ht; \
        self->ht.buf = NULL; \
        self->ht.flagbuf = NULL; \
        self->ht.size = 0; \
        self->ht.cap = 0; \
        self->ht.taken = 0; \
        return dup; \
    }

struct hash_table {
    void *buf;
    char *flagbuf;
    int64_t size;
    int64_t cap;
    int64_t taken;
    int64_t elemsz;
};
typedef struct hash_table HashTable;

void init_hash_table(HashTable *ht, int64_t elemsz, int64_t cap);
bool hash_table_insert(HashTable *ht, void *elem, uint64_t (*hash)(void*), bool (*eq)(void*, void*));
void hash_table_remove(HashTable *ht, void *iter);
void *hash_table_find(HashTable *ht, void *elem, uint64_t (*hash)(void*), bool (*eq)(void*, void*));
void *hash_table_begin(HashTable *ht);
void *hash_table_next(HashTable *ht, void *iter);
void destroy_hash_table(HashTable *ht);

#endif
