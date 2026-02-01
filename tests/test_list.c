#include "list.h"
#include "basic_types.h"

#include <assert.h>
#include <stdio.h>

LIST(IntList, Int)

int main() {
    printf("[TEST] list\n");

    IntList lst = IntList_create();

    IntList_push_back(&lst, 3);
    IntList_push_front(&lst, 1);
    IntList_insert_after(&lst, IntList_begin(&lst), 2);

    assert(lst.vhead->next->val == 1);
    assert(lst.vhead->next->next->val == 2);
    assert(lst.vhead->next->next->next->val == 3);

    assert(lst.vtail->prev->val == 3);
    assert(lst.vtail->prev->prev->val == 2);
    assert(lst.vtail->prev->prev->prev->val == 1);

    assert(IntList_len(&lst) == 3);
    assert(IntList_last(&lst)->val == 3);
    assert(IntList_begin(&lst)->val == 1);
    assert(IntList_end(&lst) == lst.vtail);

    IntList_pop_back(&lst);
    assert(lst.vhead->next->val == 1);
    assert(lst.vhead->next->next->val == 2);
    assert(lst.vtail->prev->val == 2);
    assert(lst.vtail->prev->prev->val == 1);

    IntList_pop_front(&lst);
    assert(lst.vhead->next->val == 2);
    assert(lst.vtail->prev->val == 2);

    assert(IntList_len(&lst) == 1);

    IntList_destroy(&lst);

    printf("[PASS] list\n");
}