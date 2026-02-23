#include "flat_list.h"
#include "basic_types.h"

#include <assert.h>
#include <stdio.h>

FLAT_LIST(int32_list, int32)

int main(void) {
    printf("[TEST] flat_list\n");

    int32_list_t lst = int32_list_create();

    // Test push_back and push_front
    int32_list_push_back(&lst, 3);
    int32_list_push_front(&lst, 1);

    // Test insert_after - insert 2 after 1
    int32_list_insert_after(&lst, int32_list_begin(&lst), 2);

    // Verify structure using iteration
    int32_t *val1 = int32_list_get(&lst, int32_list_begin(&lst));
    assert(*val1 == 1);

    int iter = int32_list_next(&lst, int32_list_begin(&lst));
    int32_t *val2 = int32_list_get(&lst, iter);
    assert(*val2 == 2);

    iter = int32_list_next(&lst, iter);
    int32_t *val3 = int32_list_get(&lst, iter);
    assert(*val3 == 3);

    // Verify using front and back
    assert(*int32_list_front(&lst) == 1);
    assert(*int32_list_back(&lst) == 3);

    // Test len
    assert(int32_list_len(&lst) == 3);

    // Test last
    assert(*int32_list_get(&lst, int32_list_last(&lst)) == 3);

    // Test pop_back
    int32_list_pop_back(&lst);
    assert(int32_list_len(&lst) == 2);
    assert(*int32_list_back(&lst) == 2);

    // Verify structure after pop_back
    assert(*int32_list_get(&lst, int32_list_begin(&lst)) == 1);
    assert(*int32_list_get(&lst, int32_list_last(&lst)) == 2);

    // Test pop_front
    int32_list_pop_front(&lst);
    assert(int32_list_len(&lst) == 1);
    assert(*int32_list_front(&lst) == 2);
    assert(*int32_list_back(&lst) == 2);

    // Test insert_before - insert 0 before 2
    int32_list_insert_before(&lst, int32_list_begin(&lst), 0);
    assert(int32_list_len(&lst) == 2);
    assert(*int32_list_front(&lst) == 0);
    assert(*int32_list_back(&lst) == 2);

    // Cleanup
    int32_list_destroy(&lst);

    // Test empty list operations
    int32_list_t empty = int32_list_create();
    assert(int32_list_len(&empty) == 0);
    assert(int32_list_front(&empty) == NULL);
    assert(int32_list_back(&empty) == NULL);
    int32_list_destroy(&empty);

    // Test larger list with multiple operations
    int32_list_t lst2 = int32_list_create();
    for (int i = 0; i < 5; i++) {
        int32_list_push_back(&lst2, i);
    }
    assert(int32_list_len(&lst2) == 5);

    // Verify iteration
    int expected = 0;
    for (int32_list_iter_t it = int32_list_begin(&lst2); it != int32_list_end(&lst2); it = int32_list_next(&lst2, it)) {
        int32_t *val = int32_list_get(&lst2, it);
        assert(*val == expected);
        expected++;
    }
    assert(expected == 5);

    int32_list_destroy(&lst2);

    printf("[PASS] flat_list\n");
    return 0;
}
