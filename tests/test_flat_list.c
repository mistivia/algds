#include "flat_list.h"
#include "basic_types.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

FLAT_LIST(int32_list, int32)
FLAT_LIST(str_list, str)

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

    str_list_t s_lst = str_list_create();

    // Test push_back and push_front with strings
    str_list_push_back(&s_lst, strdup("world"));
    str_list_push_front(&s_lst, strdup("hello"));

    // Verify structure
    assert(strcmp(*str_list_front(&s_lst), "hello") == 0);
    assert(strcmp(*str_list_back(&s_lst), "world") == 0);
    assert(str_list_len(&s_lst) == 2);

    // Test insert_after - insert "beautiful" after "hello"
    str_list_insert_after(&s_lst, str_list_begin(&s_lst), strdup("beautiful"));
    assert(str_list_len(&s_lst) == 3);

    // Verify order: hello -> beautiful -> world
    str_list_iter_t it = str_list_begin(&s_lst);
    assert(strcmp(*str_list_get(&s_lst, it), "hello") == 0);
    it = str_list_next(&s_lst, it);
    assert(strcmp(*str_list_get(&s_lst, it), "beautiful") == 0);
    it = str_list_next(&s_lst, it);
    assert(strcmp(*str_list_get(&s_lst, it), "world") == 0);

    // Test insert_before
    it = str_list_last(&s_lst);
    str_list_insert_before(&s_lst, it, strdup("cruel"));
    assert(str_list_len(&s_lst) == 4);

    // Verify order: hello -> beautiful -> cruel -> world
    it = str_list_begin(&s_lst);
    assert(strcmp(*str_list_get(&s_lst, it), "hello") == 0);
    it = str_list_next(&s_lst, it);
    assert(strcmp(*str_list_get(&s_lst, it), "beautiful") == 0);
    it = str_list_next(&s_lst, it);
    assert(strcmp(*str_list_get(&s_lst, it), "cruel") == 0);
    it = str_list_next(&s_lst, it);
    assert(strcmp(*str_list_get(&s_lst, it), "world") == 0);

    // Test pop_front
    str_list_pop_front(&s_lst);
    assert(str_list_len(&s_lst) == 3);
    assert(strcmp(*str_list_front(&s_lst), "beautiful") == 0);

    // Test pop_back
    str_list_pop_back(&s_lst);
    assert(str_list_len(&s_lst) == 2);
    assert(strcmp(*str_list_back(&s_lst), "cruel") == 0);

    // Test iteration over remaining elements
    char *expected_strs[] = {"beautiful", "cruel"};
    int idx = 0;
    for (str_list_iter_t iter = str_list_begin(&s_lst); iter != str_list_end(&s_lst); iter = str_list_next(&s_lst, iter)) {
        assert(strcmp(*str_list_get(&s_lst, iter), expected_strs[idx]) == 0);
        idx++;
    }
    assert(idx == 2);

    // Cleanup (str_list_destroy will call str_destroy on each element)
    str_list_destroy(&s_lst);

    printf("[PASS] flat_list\n");
    return 0;
}
