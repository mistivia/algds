#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <basic_types.h>

void test_str_eq() {
    str_t s1 = "hello";
    str_t s2 = "hello";
    str_t s3 = "world";

    assert(str_eq(&s1, &s2) == true);
    assert(str_eq(&s1, &s3) == false);
    assert(str_eq(&s1, &s1) == true);

    str_t empty1 = "";
    str_t empty2 = "";
    assert(str_eq(&empty1, &empty2) == true);
    assert(str_eq(&empty1, &s1) == false);

    printf("  [PASS] str_eq\n");
}

void test_str_cmp() {
    str_t s1 = "abc";
    str_t s2 = "abc";
    str_t s3 = "def";
    str_t s4 = "abd";

    assert(str_cmp(&s1, &s2) == 0);
    assert(str_cmp(&s1, &s3) < 0);
    assert(str_cmp(&s3, &s1) > 0);
    assert(str_cmp(&s1, &s4) < 0);

    str_t empty = "";
    str_t abc = "abc";
    assert(str_cmp(&empty, &abc) < 0);
    assert(str_cmp(&abc, &empty) > 0);
    assert(str_cmp(&empty, &empty) == 0);

    str_t lower = "abc";
    str_t upper = "ABC";
    assert(str_cmp(&lower, &upper) > 0);

    printf("  [PASS] str_cmp\n");
}

void test_str_copy() {
    str_t original = "hello world";
    str_t copy = str_copy(&original);

    assert(strcmp(copy, original) == 0);
    assert(copy != original);

    str_destroy(&copy);

    str_t empty = "";
    str_t empty_copy = str_copy(&empty);
    assert(strcmp(empty_copy, "") == 0);
    str_destroy(&empty_copy);

    printf("  [PASS] str_copy\n");
}

void test_str_hash() {
    str_t s1 = "hello";
    str_t s2 = "hello";
    str_t s3 = "world";

    uint64_t hash1 = str_hash(&s1);
    uint64_t hash2 = str_hash(&s2);
    assert(hash1 == hash2);

    uint64_t hash3 = str_hash(&s3);
    assert(hash1 != hash3);

    str_t empty = "";
    uint64_t hash_empty = str_hash(&empty);
    assert(hash_empty == str_hash(&empty));

    str_t long_str = "this is a very long string to test hashing";
    uint64_t hash_long = str_hash(&long_str);
    assert(hash_long == str_hash(&long_str));

    printf("  [PASS] str_hash\n");
}

void test_str_destroy() {
    str_t original = "test string";
    str_t copy = str_copy(&original);

    assert(strcmp(copy, original) == 0);

    str_destroy(&copy);

    char *dynamic = strdup("dynamic string");
    str_t s = dynamic;
    str_destroy(&s);

    printf("  [PASS] str_destroy\n");
}

void test_str_all() {
    str_t s1 = "algds";
    str_t s2 = str_copy(&s1);

    assert(str_eq(&s1, &s2) == true);
    assert(str_cmp(&s1, &s2) == 0);
    assert(str_hash(&s1) == str_hash(&s2));

    str_destroy(&s2);

    printf("  [PASS] str_all (integration)\n");
}

int main() {
    printf("[TEST] basic_types\n");

    test_str_eq();
    test_str_cmp();
    test_str_copy();
    test_str_hash();
    test_str_destroy();
    test_str_all();

    printf("[PASS] basic_types\n");
    return 0;
}
