#include "vec.h"

#include <assert.h>
#include <stdio.h>

int main() {
    printf("[TEST] vec\n");

    IntVector vec;
    IntVector_init(&vec);

    for (int i = 0; i < 1000; i++) {
        if (i == 10) {
            IntVector_show(&vec, stdout);
            puts("");
        }
        assert(vec.size == i);
        IntVector_push_back(&vec, i);
        assert(*(IntVector_end(&vec) - 1) == i);
    }
    assert(*IntVector_begin(&vec) == 0);

    IntVector_free(&vec);
    printf("[PASS] vec\n");
    return 0;
}
