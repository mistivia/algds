#include "hash_table.h"

#include <stdlib.h>
#include <string.h>

#define HTFL_NUL 0
#define HTFL_VAL 1
#define HTFL_DEL 2


static void rebuild(hash_table_t *ht) {
    hash_table_t newht;
    init_hash_table(&newht, ht->elemsz, ht->size * 6, ht->hash, ht->eq);
    void *iter = hash_table_begin(ht);
    while (iter != NULL) {
        hash_table_insert(&newht, iter);
        iter = hash_table_next(ht, iter);
    }
    free(ht->buf);
    free(ht->flagbuf);
    *ht = newht;
}

void init_hash_table(hash_table_t *ht, int64_t elemsz, int64_t cap,
                     uint64_t (*hash)(void *), bool (*eq)(void *, void *)) {
    if (cap < 16) cap = 16;
    ht->buf = malloc(cap * elemsz);
    ht->flagbuf = malloc(cap);
    memset(ht->buf, 0, cap * elemsz);
    memset(ht->flagbuf, 0, cap);
    ht->size = 0;
    ht->cap = cap;
    ht->taken = 0;
    ht->elemsz = elemsz;
    ht->hash = hash;
    ht->eq = eq;
}

bool hash_table_insert(hash_table_t *ht, void *elem) {
    if (ht->taken + 1 > ht->cap / 2) {
        rebuild(ht);
    }
    ht->taken++;
    ht->size++;
    int64_t pos = ht->hash(elem) % ht->cap;
    while (ht->flagbuf[pos] != HTFL_NUL) {
        if (ht->flagbuf[pos] == HTFL_VAL
                && ht->eq(ht->buf + pos * ht->elemsz, elem)) {
            return false;
        }
        pos++; 
        if (pos >= ht->cap) { // arrived end, restart from beginning
            pos = 0;
        }
    }
    memcpy(ht->buf + pos * ht->elemsz, elem, ht->elemsz);
    ht->flagbuf[pos] = HTFL_VAL;
    return true;
}

void hash_table_remove(hash_table_t *ht, void * ptr) {
    ht->size--;
    int64_t pos = (ptr - ht->buf) / ht->elemsz;
    ht->flagbuf[pos] = HTFL_DEL;
}

void *hash_table_ref(hash_table_t *ht, int64_t pos) {
    return ht->buf + pos * ht->elemsz;
}

void *hash_table_find(hash_table_t *ht, void *elem) {
    int64_t pos = ht->hash(elem) % ht->cap;
    while (ht->flagbuf[pos] != HTFL_NUL) {
        if (ht->flagbuf[pos] == HTFL_VAL
                && ht->eq(hash_table_ref(ht, pos), elem)) {
            return hash_table_ref(ht, pos);
        }
        pos++;
        if (pos >= ht->cap) { // arrived end, restart from beginning
            pos = 0;
        }
    }
    return NULL;
}

void* hash_table_begin(hash_table_t *ht) {
    if (ht->size <= 0) return NULL;
    for (int64_t i = 0; i < ht->cap; i++) {
        if (ht->flagbuf[i] == HTFL_VAL) {
            return hash_table_ref(ht, i);
        }
    }
    return NULL;
}

void *hash_table_next(hash_table_t *ht, void *iter) {
    int64_t pos = (iter - ht->buf) / ht->elemsz;
    do {
        pos++;
        if (pos >= ht->cap) {
            return NULL;
        }
    } while (ht->flagbuf[pos] != HTFL_VAL);
    return hash_table_ref(ht, pos);
}

void destroy_hash_table(hash_table_t *ht) {
    free(ht->buf);
    free(ht->flagbuf);
}
