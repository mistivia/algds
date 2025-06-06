#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"
#include "mmhash.h"

static uint64_t hash(void *i) { return mmhash(i, sizeof(int), 0); }

static bool eq(void *x, void *y) {
    int *a = x, *b = y;
    return *a == *b;
}

bool found[10000];

int main() {
    printf("[TEST] htable\n");

    hash_table_t ht;
    init_hash_table(&ht, sizeof(int), -1, hash, eq);
    for (int i = 0; i < 10000; i++) {
        hash_table_insert(&ht, &i);
        assert(ht.size == i + 1);
        assert(ht.taken == i + 1);
        assert(ht.cap >= i + 1);
    }

    for (int i = 0; i < 10000; i++) {
        assert(hash_table_find(&ht, &i) != NULL);
        int t = 10000 + i;
        assert(hash_table_find(&ht, &t) == NULL);
    }

    memset(found, 0, sizeof(bool) * 10000);
    int *iter = hash_table_begin(&ht);
    while (iter != NULL) {
        found[*iter] = true;
        iter = hash_table_next(&ht, iter);
    }
    for (int i = 0; i < 10000; i++) {
        assert(found[i]);
    }

    for (int i = 0; i < 5000; i++) {
        int *iter = hash_table_find(&ht, &i);
        hash_table_remove(&ht, iter);
    }
    for (int i = 0; i < 5000; i++) {
        assert(hash_table_find(&ht, &i) == NULL);
        int t = 5000 + i;
        assert(hash_table_find(&ht, &t) != NULL);
    }

    for (int i = 0; i < 5000; i++) {
        hash_table_insert(&ht, &i);
    }

    memset(found, 0, sizeof(bool) * 10000);
    iter = hash_table_begin(&ht);
    while (iter != NULL) {
        found[*iter] = true;
        iter = hash_table_next(&ht, iter);
    }
    for (int i = 0; i < 10000; i++) {
        assert(found[i]);
    }
    destroy_hash_table(&ht);

    printf("[PASS] htable\n");
}
