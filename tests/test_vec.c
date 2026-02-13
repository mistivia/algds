#include "vec.h"

#include <assert.h>
#include <stdio.h>

#include "basic_types.h"
#include "vec.h"

VECTOR(int32_vec, int32)

int main() {
    printf("[TEST] vec\n");

    int32_vec_t vec = int32_vec_create();

    for (int i = 0; i < 1000; i++) {
        assert(vec.size == i);
        int32_vec_push_back(&vec, i);
        assert(*(int32_vec_end(&vec) - 1) == i);
    }
    assert(*int32_vec_begin(&vec) == 0);

    int32_vec_destroy(&vec);
    printf("[PASS] vec\n");
    return 0;
}
