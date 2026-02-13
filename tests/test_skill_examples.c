#include <stdio.h>
#include <assert.h>
#include "basic_types.h"
#include "vec.h"

VECTOR(int_vec, int32)

void test_quick_start() {
    int_vec_t vec = int_vec_create();
    int_vec_push_back(&vec, 42);
    int32_t *ptr = int_vec_ref(&vec, 0);
    printf("vec[0] = %d\n", *ptr);
    assert(*ptr == 42);
    assert(int_vec_len(&vec) == 1);
    int_vec_destroy(&vec);
}

#include "list.h"
#include "hash_table.h"
#include "pqueue.h"
#include "sort.h"

LIST(int_list, int32)
HASH_TABLE(int_map, int32, int32)
PQUEUE(int_pq, int32)

void test_full_examples() {
    // Vector
    printf("\n--- Vector ---\n");
    int_vec_t vec = int_vec_create();
    for (int i = 0; i < 5; i++) int_vec_push_back(&vec, i);
    printf("Vector: ");
    for (int32_t *it = int_vec_begin(&vec); it != int_vec_end(&vec); it++)
        printf("%d ", *it);
    printf("\n");
    assert(int_vec_len(&vec) == 5);
    assert(*int_vec_ref(&vec, 0) == 0);
    assert(*int_vec_ref(&vec, 4) == 4);
    int_vec_destroy(&vec);
    
    // List
    printf("\n--- List ---\n");
    int_list_t lst = int_list_create();
    int_list_push_back(&lst, 1);
    int_list_push_back(&lst, 2);
    printf("List: ");
    for (int_list_iter_t it = int_list_begin(&lst); it != int_list_end(&lst); it = int_list_next(&lst, it))
        printf("%d ", it->val);
    printf("\n");
    assert(int_list_len(&lst) == 2);
    int_list_destroy(&lst);
    
    // Hash Table
    printf("\n--- Hash Table ---\n");
    int_map_t ht = int_map_create();
    int_map_insert(&ht, 1, 10);
    int_map_insert(&ht, 2, 20);
    int32_t *val = int_map_get(&ht, 1);
    printf("ht[1] = %d\n", *val);
    assert(*val == 10);
    val = int_map_get(&ht, 2);
    printf("ht[2] = %d\n", *val);
    assert(*val == 20);
    assert(int_map_get(&ht, 999) == NULL);
    int_map_destroy(&ht);
    
    // Priority Queue
    int_pq_t pq = int_pq_create();
    int_pq_push(&pq, 5);
    int_pq_push(&pq, 10);
    int_pq_push(&pq, 3);
    printf("Priority Queue (max-heap): ");
    int expected[] = {10, 5, 3};
    int idx = 0;
    while (int_pq_top(&pq) != NULL) {
        printf("%d ", *int_pq_top(&pq));
        assert(*int_pq_top(&pq) == expected[idx++]);
        int_pq_pop(&pq);
    }
    printf("\n");
    int_pq_destroy(&pq);
    
    // Sort
    int32_t arr[] = {5, 2, 8, 1, 9};
    printf("Before sort: ");
    for (int i = 0; i < 5; i++) printf("%d ", arr[i]);
    printf("\n");
    
    int32_qsort(arr, 5);
    
    printf("After sort:  ");
    for (int i = 0; i < 5; i++) printf("%d ", arr[i]);
    printf("\n");
    
    assert(arr[0] == 1);
    assert(arr[1] == 2);
    assert(arr[2] == 5);
    assert(arr[3] == 8);
    assert(arr[4] == 9);
}

int main() {
    printf("[TEST] skill example\n");
    test_quick_start();
    test_full_examples();
    printf("[PASS] skill example\n");
    return 0;
}
