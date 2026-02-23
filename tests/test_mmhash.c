#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

int main() {
    printf("[TEST] hash\n");
    char buf[] = "hello, world\n";
    uint64_t hash0 = mmhash(buf, strlen(buf), 0);
    uint64_t hash1 = mmhash(buf, 3, 0);
    assert(hash0 != hash1);
    printf("[PASS] hash\n");
    return 0;
}
