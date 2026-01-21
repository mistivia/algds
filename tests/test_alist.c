#include "alist.h"
#include "basic_traits.h"

#include <assert.h>
#include <stdio.h>

int main() {
    printf("[TEST] alist\n");

    IntAList lst;
    IntAList_init(&lst);

    IntAList_push_back(&lst, 3);
    IntAList_push_front(&lst, 1);
    IntAList_insert_after(&lst, IntAList_begin(&lst), 2);

    printf("List content: ");
    IntAList_show(&lst, stdout);
    printf("\n");

    // Current list: 1, 2, 3
    IntAListIter it = IntAList_begin(&lst);
    assert(*IntAList_get(&lst, it) == 1);
    it = IntAList_next(&lst, it);
    assert(*IntAList_get(&lst, it) == 2);
    it = IntAList_next(&lst, it);
    assert(*IntAList_get(&lst, it) == 3);
    it = IntAList_next(&lst, it);
    assert(it == IntAList_end(&lst));

    assert(IntAList_len(&lst) == 3);
    assert(*IntAList_get(&lst, IntAList_last(&lst)) == 3);
    assert(*IntAList_get(&lst, IntAList_begin(&lst)) == 1);

    IntAList_pop_back(&lst);
    // Current list: 1, 2
    assert(IntAList_len(&lst) == 2);
    assert(*IntAList_get(&lst, IntAList_last(&lst)) == 2);

    IntAList_pop_front(&lst);
    // Current list: 2
    assert(IntAList_len(&lst) == 1);
    assert(*IntAList_get(&lst, IntAList_begin(&lst)) == 2);

    IntAList_push_back(&lst, 6);
    IntAList_push_back(&lst, 7);
    // 2, 6, 7
    it = IntAList_last(&lst);
    assert(*IntAList_get(&lst, it) == 7);
    it = IntAList_prev(&lst, it);
    assert(*IntAList_get(&lst, it) == 6);
    it = IntAList_prev(&lst, it);
    assert(*IntAList_get(&lst, it) == 2);
    it = IntAList_prev(&lst, it);
    assert(it == -1);

    // Test empty list reuse
    int idx1 = IntAList_push_back(&lst, 4);
    IntAList_remove(&lst, idx1);
    int idx2 = IntAList_push_back(&lst, 5);
    assert(idx1 == idx2); // Should reuse the index

    // Test insert_before
    // Current: 2, 6, 7, 5
    IntAList_insert_before(&lst, IntAList_begin(&lst), 0);
    // Current: 0, 2, 6, 7, 5
    assert(*IntAList_get(&lst, IntAList_begin(&lst)) == 0);
    assert(IntAList_len(&lst) == 5);

    // Test removal from middle
    // Remove 6
    IntAListIter it6 = IntAList_next(&lst, IntAList_next(&lst, IntAList_begin(&lst)));
    assert(*IntAList_get(&lst, it6) == 6);
    IntAList_remove(&lst, it6);
    // Current: 0, 2, 7, 5
    assert(IntAList_len(&lst) == 4);
    it = IntAList_begin(&lst);
    assert(*IntAList_get(&lst, it) == 0); it = IntAList_next(&lst, it);
    assert(*IntAList_get(&lst, it) == 2); it = IntAList_next(&lst, it);
    assert(*IntAList_get(&lst, it) == 7); it = IntAList_next(&lst, it);
    assert(*IntAList_get(&lst, it) == 5);

    // Test AList_move
    IntAList lst2 = IntAList_move(&lst);
    assert(IntAList_len(&lst) == 0);
    assert(IntAList_len(&lst2) == 4);
    assert(*IntAList_get(&lst2, IntAList_begin(&lst2)) == 0);
    IntAList_free(&lst); // Should be safe

    // Test large number of elements
    for (int i = 0; i < 100; i++) {
        IntAList_push_back(&lst2, i + 100);
    }
    assert(IntAList_len(&lst2) == 104);
    
    // Clear by popping
    while (IntAList_len(&lst2) > 0) {
        IntAList_pop_front(&lst2);
    }
    assert(IntAList_len(&lst2) == 0);
    assert(IntAList_begin(&lst2) == IntAList_end(&lst2));

    IntAList_free(&lst2);

    printf("[PASS] alist\n");
    return 0;
}
