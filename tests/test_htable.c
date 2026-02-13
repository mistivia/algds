#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"
#include "basic_types.h"

HASH_TABLE(int32_ht, int32, int32)

bool found[10000];

int main() {
    printf("[TEST] htable\n");

    int32_ht_t ht;
    int32_ht_init(&ht);
    for (int i = 0; i < 10000; i++) {
        int32_ht_insert(&ht, i, i*2);
        assert(int32_ht_len(&ht) == (size_t)(i + 1));
        assert(ht.ht.taken == i + 1);
        assert(ht.ht.cap >= i + 1);
    }

    for (int i = 0; i < 10000; i++) {
        assert(int32_ht_get(&ht, i) != NULL);
        assert(*int32_ht_get(&ht, i) == i * 2);
        int t = 10000 + i;
        assert(int32_ht_get(&ht, t) == NULL);
    }

    memset(found, 0, sizeof(bool) * 10000);
    int32_ht_iter_t iter = int32_ht_begin(&ht);
    while (iter != int32_ht_end(&ht)) {
        found[iter->key] = true;
        iter = int32_ht_next(&ht, iter);
    }
    for (int i = 0; i < 10000; i++) {
        assert(found[i]);
    }

    for (int i = 0; i < 5000; i++) {
        int32_ht_iter_t it = int32_ht_find(&ht, i);
        int32_ht_remove(&ht, it);
    }
    for (int i = 0; i < 5000; i++) {
        assert(int32_ht_find(&ht, i) == int32_ht_end(&ht));
        int t = 5000 + i;
        assert(int32_ht_find(&ht, t) != int32_ht_end(&ht));
    }

    for (int i = 0; i < 5000; i++) {
        int32_ht_insert(&ht, i, i);
    }

    memset(found, 0, sizeof(bool) * 10000);
    iter = int32_ht_begin(&ht);
    while (iter != int32_ht_end(&ht)) {
        found[iter->key] = true;
        iter = int32_ht_next(&ht, iter);
    }
    for (int i = 0; i < 10000; i++) {
        assert(found[i]);
    }
    int32_ht_destroy(&ht);

    printf("[PASS] htable\n");
}
