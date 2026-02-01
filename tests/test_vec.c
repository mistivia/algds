#include "vec.h"

#include <assert.h>
#include <stdio.h>

#include "basic_types.h"
#include "vec.h"

VECTOR(IntVector, Int)

int main() {
    printf("[TEST] vec\n");

    IntVector vec = IntVector_create();

    for (int i = 0; i < 1000; i++) {
        assert(vec.size == i);
        IntVector_push_back(&vec, i);
        assert(*(IntVector_end(&vec) - 1) == i);
    }
    assert(*IntVector_begin(&vec) == 0);

    IntVector_destroy(&vec);
    printf("[PASS] vec\n");
    return 0;
}
