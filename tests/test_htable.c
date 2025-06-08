#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

bool found[10000];

int main() {
    printf("[TEST] htable\n");

    Int2IntHashTable ht;
    Int2IntHashTable_init(&ht);
    for (int i = 0; i < 10000; i++) {
        Int2IntHashTable_insert(&ht, &i, &i);
        assert(ht.ht.size == i + 1);
        assert(ht.ht.taken == i + 1);
        assert(ht.ht.cap >= i + 1);
    }

    for (int i = 0; i < 10000; i++) {
        assert(Int2IntHashTable_get(&ht, &i) != NULL);
        int t = 10000 + i;
        assert(Int2IntHashTable_get(&ht, &t) == NULL);
    }

    memset(found, 0, sizeof(bool) * 10000);
    Int2IntHashTableIter iter = Int2IntHashTable_begin(&ht);
    while (iter != NULL) {
        found[iter->key] = true;
        iter = Int2IntHashTable_next(&ht, iter);
    }
    for (int i = 0; i < 10000; i++) {
        assert(found[i]);
    }

    for (int i = 0; i < 5000; i++) {
        Int2IntHashTableIter iter = Int2IntHashTable_find(&ht, &i);
        Int2IntHashTable_remove(&ht, iter);
    }
    for (int i = 0; i < 5000; i++) {
        assert(Int2IntHashTable_find(&ht, &i) == NULL);
        int t = 5000 + i;
        assert(Int2IntHashTable_find(&ht, &t) != NULL);
    }

    for (int i = 0; i < 5000; i++) {
        Int2IntHashTable_insert(&ht, &i, &i);
    }

    memset(found, 0, sizeof(bool) * 10000);
    iter = Int2IntHashTable_begin(&ht);
    while (iter != NULL) {
        found[iter->key] = true;
        iter = Int2IntHashTable_next(&ht, iter);
    }
    for (int i = 0; i < 10000; i++) {
        assert(found[i]);
    }
    Int2IntHashTable_free(&ht);

    printf("[PASS] htable\n");
}
