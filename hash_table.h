#ifndef HTABLE_H_
#define HTABLE_H_

#include <stdbool.h>
#include <stdint.h>

#define HASH_TABLE(A, K, V) \
    typedef struct { \
        K key; \
        V val; \
    } A##Entry_; \
    typedef A##Entry_ *A##Iter; \
    typedef struct { \
        HashTable ht; \
    } A; \
    A A##_create()  __attribute__((weak)); \
    bool A##_insert(A *self, K key, V value)  __attribute__((weak)); \
    void A##_remove(A *ht, A##Iter iter)  __attribute__((weak)); \
    V* A##_get(A *self, K key)  __attribute__((weak)); \
    A##Iter A##_find(A *self, K key)  __attribute__((weak)); \
    A##Iter A##_begin(A *self)  __attribute__((weak)); \
    A##Iter A##_end(A *self)  __attribute__((weak)); \
    A##Iter A##_next(A *self, A##Iter iter)  __attribute__((weak)); \
    void A##_destroy(A *self)  __attribute__((weak)); \
    A A##_move(A *self)  __attribute__((weak)); \
    \
    static inline uint64_t A##_hash(void *vk) { \
        K *k = vk; \
        return K##_hash(*k); \
    } \
    static inline bool A##_eq(void *vk1, void *vk2) { \
        K *k1 = vk1, *k2 = vk2; \
        return K##_eq(*k1, *k2); \
    } \
    static inline void A##_init(A *self) { \
        init_hash_table(&self->ht, sizeof(A##Entry_), 16); \
    } \
    A A##_create() { \
        A self; \
        A##_init(&self); \
        return self; \
    } \
    bool A##_insert(A *self, K key, V value) { \
        A##Entry_ entry; \
        entry.key = key; \
        entry.val = value; \
        return hash_table_insert(&self->ht, &entry, A##_hash, A##_eq); \
    } \
    A##Iter A##_find(A *self, K key) { \
        return hash_table_find(&self->ht, &key, A##_hash, A##_eq); \
    } \
    V* A##_get(A *self, K key) { \
        A##Entry_* entry = hash_table_find(&self->ht, &key, A##_hash, A##_eq); \
        if (entry == NULL) return NULL; \
        return &(entry->val); \
    } \
    void A##_remove(A *self, A##Iter iter) { \
        K##_destroy(&iter->key); \
        V##_destroy(&iter->val); \
        hash_table_remove(&self->ht, iter); \
    } \
    A##Iter A##_begin(A *self) { \
        return hash_table_begin(&self->ht); \
    } \
    A##Iter A##_next(A *self, A##Iter iter) { \
        return hash_table_next(&self->ht, iter); \
    } \
    void A##_destroy(A *self) { \
        A##Iter it = A##_begin(self); \
        while (it != A##_end(self)) { \
            K##_destroy(&it->key); \
            V##_destroy(&it->val); \
            it = A##_next(self, it); \
        } \
        destroy_hash_table(&self->ht); \
    } \
    A A##_move(A *self) { \
        A dup; \
        dup.ht = self->ht; \
        self->ht.buf = NULL; \
        self->ht.flagbuf = NULL; \
        self->ht.size = 0; \
        self->ht.cap = 0; \
        self->ht.taken = 0; \
        return dup; \
    } \
    A##Iter A##_end(A *self) {return NULL;}  \
    \

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
