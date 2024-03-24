#include "hash_table.h"

#include <stdlib.h>
#include <string.h>

#define HTFL_NUL 0
#define HTFL_VAL 1
#define HTFL_DEL 2

static void *hash_table_end(hash_table_t *ht) {
    return ht->buf + ht->cap * (ht->elemsz + 1);
}

static void rebuild(hash_table_t *ht) {
    hash_table_t newht;
    init_hash_table(&newht, ht->elemsz, ht->size * 6, ht->hash, ht->eq);
    void *iter = hash_table_begin(ht);
    while (iter != NULL) {
        hash_table_insert(&newht, iter);
        iter = hash_table_next(ht, iter);
    }
    free(ht->buf);
    *ht = newht;
}

static uint8_t getflag(void *iter) { return *(uint8_t *)(iter - 1); }

static void setflag(void *iter, uint8_t flag) { *(uint8_t *)(iter - 1) = flag; }

void init_hash_table(hash_table_t *ht, int64_t elemsz, int64_t cap,
                     uint64_t (*hash)(void *), bool (*eq)(void *, void *)) {
    if (cap < 16) cap = 16;
    ht->buf = malloc(cap * (elemsz + 1));
    memset(ht->buf, 0, cap * (elemsz + 1));
    ht->size = 0;
    ht->cap = cap;
    ht->taken = 0;
    ht->begin = NULL;
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
    int64_t hashcode = ht->hash(elem) % ht->cap;
    void *pos = ht->buf + hashcode * (ht->elemsz + 1) + 1;
    while (getflag(pos) != HTFL_NUL) {
        if (getflag(pos) == HTFL_VAL && ht->eq(pos, elem)) return false;
        pos += ht->elemsz + 1;
        if (pos >= hash_table_end(ht)) { // arrived end, restart from beginning
            pos = ht->buf + 1;
        }
    }
    memcpy(pos, elem, ht->elemsz);
    setflag(pos, HTFL_VAL);
    if (pos < ht->begin || ht->begin == NULL) {
        ht->begin = pos;
    }
    return true;
}

void hash_table_remove(hash_table_t *ht, void *iter) {
    ht->size--;
    setflag(iter, HTFL_DEL);
    if (iter == ht->begin) {
        ht->begin = hash_table_next(ht, iter);
    }
}

void *hash_table_find(hash_table_t *ht, void *elem) {
    int64_t hashcode = ht->hash(elem) % ht->cap;
    void *pos = ht->buf + hashcode * (ht->elemsz + 1) + 1;
    while (getflag(pos) != HTFL_NUL) {
        if (getflag(pos) == HTFL_VAL && ht->eq(pos, elem)) return pos;
        pos += ht->elemsz + 1;
        if (pos >= hash_table_end(ht)) { // arrived end, restart from beginning
            pos = ht->buf + 1;
        }
    }
    return NULL;
}

void *hash_table_begin(hash_table_t *ht) { return ht->begin; }

void *hash_table_next(hash_table_t *ht, void *iter) {
    void *pos = iter;
    do {
        pos += ht->elemsz + 1;
        if (pos >= hash_table_end(ht)) {
            return NULL;
        }
    } while (getflag(pos) != HTFL_VAL);
    return pos;
}
