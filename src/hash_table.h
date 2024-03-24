#ifndef HTABLE_H_
#define HTABLE_H_

#include <stdbool.h>
#include <stdint.h>

struct hash_table {
    void *buf;
    int64_t size;
    int64_t cap;
    int64_t taken;
    void *begin;
    int64_t elemsz;
    uint64_t (*hash)(void *);
    bool (*eq)(void *, void *);
};
typedef struct hash_table hash_table_t;

void init_hash_table(hash_table_t *ht, int64_t elemsz, int64_t cap,
                     uint64_t (*hash)(void *), bool (*eq)(void *, void *));
bool hash_table_insert(hash_table_t *ht, void *elem);
void hash_table_remove(hash_table_t *ht, void *iter);

// return a iterator
void *hash_table_find(hash_table_t *ht, void *elem);
void *hash_table_begin(hash_table_t *ht);
void *hash_table_next(hash_table_t *ht, void *iter);

#endif
