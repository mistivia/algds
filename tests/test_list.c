#include "list.h"
#include "basic_types.h"

#include <assert.h>
#include <stdio.h>

LIST(int32_list, int32)

int main() {
    printf("[TEST] list\n");

    int32_list_t lst = int32_list_create();

    int32_list_push_back(&lst, 3);
    int32_list_push_front(&lst, 1);
    int32_list_insert_after(&lst, int32_list_begin(&lst), 2);

    assert(lst.vhead->next->val == 1);
    assert(lst.vhead->next->next->val == 2);
    assert(lst.vhead->next->next->next->val == 3);

    assert(lst.vtail->prev->val == 3);
    assert(lst.vtail->prev->prev->val == 2);
    assert(lst.vtail->prev->prev->prev->val == 1);

    assert(int32_list_len(&lst) == 3);
    assert(int32_list_last(&lst)->val == 3);
    assert(int32_list_begin(&lst)->val == 1);
    assert(int32_list_end(&lst) == lst.vtail);

    int32_list_pop_back(&lst);
    assert(lst.vhead->next->val == 1);
    assert(lst.vhead->next->next->val == 2);
    assert(lst.vtail->prev->val == 2);
    assert(lst.vtail->prev->prev->val == 1);

    int32_list_pop_front(&lst);
    assert(lst.vhead->next->val == 2);
    assert(lst.vtail->prev->val == 2);

    assert(int32_list_len(&lst) == 1);

    int32_list_destroy(&lst);

    printf("[PASS] list\n");
    return 0;
}
