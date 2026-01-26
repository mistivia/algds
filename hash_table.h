#ifndef HTABLE_H_
#define HTABLE_H_

#include <stdbool.h>
#include <stdint.h>

#include "type_alias.h"

struct hash_table {
    void *buf;
    char *flagbuf;
    int64_t size;
    int64_t cap;
    int64_t taken;
    int64_t elemsz;
};
typedef struct hash_table HashTable;

#define DEF_HASH_TABLE(K, V, A) \
    typedef struct { \
        K key; \
        V val; \
    } A##Entry_; \
    typedef A##Entry_ *A##Iter; \
    typedef struct { \
        HashTable ht; \
    } A; \
    void A##_init(A *self); \
    A A##_create(); \
    bool A##_insert(A *self, K key, V value); \
    void A##_remove(A *ht, A##Iter iter); \
    V* A##_get(A *self, K key); \
    A##Iter A##_find(A *self, K key); \
    A##Iter A##_begin(A *self); \
    A##Iter A##_next(A *self, A##Iter iter); \
    void A##_free(A *self); \
    A A##_move(A *self);

#define HASH_TABLE_DEF(K, V) DEF_HASH_TABLE(K, V, K##2##V##HashTable)

#define IMPL_HASH_TABLE(K, V, A) \
    static uint64_t A##_hash(void *vk) { \
        K *k = vk; \
        return K##_hash(*k); \
    } \
    static bool A##_eq(void *vk1, void *vk2) { \
        K *k1 = vk1, *k2 = vk2; \
        return K##_eq(*k1, *k2); \
    } \
    void A##_init(A *self) { \
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
        hash_table_remove(&self->ht, iter); \
    } \
    A##Iter A##_begin(A *self) { \
        return hash_table_begin(&self->ht); \
    } \
    A##Iter A##_next(A *self, A##Iter iter) { \
        return hash_table_next(&self->ht, iter); \
    } \
    void A##_free(A *self) { \
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
    }

#define HASH_TABLE_IMPL(K, V) IMPL_HASH_TABLE(K, V, K##2##V##HashTable)

HASH_TABLE_DEF(String, Int);
HASH_TABLE_DEF(String, String);
HASH_TABLE_DEF(String, Double);
HASH_TABLE_DEF(String, VoidPtr);
HASH_TABLE_DEF(Int, Int);
HASH_TABLE_DEF(Int, Double);
HASH_TABLE_DEF(VoidPtr, Int);
HASH_TABLE_DEF(VoidPtr, String);
HASH_TABLE_DEF(VoidPtr, VoidPtr);

void init_hash_table(HashTable *ht, int64_t elemsz, int64_t cap);
bool hash_table_insert(HashTable *ht, void *elem, uint64_t (*hash)(void*), bool (*eq)(void*, void*));
void hash_table_remove(HashTable *ht, void *iter);
void *hash_table_find(HashTable *ht, void *elem, uint64_t (*hash)(void*), bool (*eq)(void*, void*));
void *hash_table_begin(HashTable *ht);
void *hash_table_next(HashTable *ht, void *iter);
void destroy_hash_table(HashTable *ht);

#endif
