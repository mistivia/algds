#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <str.h>

void test_str_split() {
    char *s = "abc 123 233 xyz";
    char **list = str_split(s, ' ');
    assert(list[4] == NULL);
    assert(list[3] != NULL);
    assert(strcmp(list[0], "abc") == 0);
    assert(strcmp(list[3], "xyz") == 0);
    destroy_str_list(list);

    s = "abc  123 233 xyz";
    list = str_split(s, ' ');
    assert(list[4] == NULL);
    assert(list[3] != NULL);
    assert(strcmp(list[0], "abc") == 0);
    assert(strcmp(list[3], "xyz") == 0);
    destroy_str_list(list);

    s = "   abc  123 233 xyz";
    list = str_split(s, ' ');
    assert(list[4] == NULL);
    assert(list[3] != NULL);
    assert(strcmp(list[0], "abc") == 0);
    assert(strcmp(list[3], "xyz") == 0);
    destroy_str_list(list);

    s = "   abc \t 123\n 233\nxyz";
    list = str_split(s, '\0');
    assert(list[4] == NULL);
    assert(list[3] != NULL);
    assert(strcmp(list[0], "abc") == 0);
    assert(strcmp(list[3], "xyz") == 0);
    destroy_str_list(list);

    s = "a";
    list = str_split(s, ' ');
    assert(list[1] == NULL);
    assert(list[0] != NULL);
    assert(strcmp(list[0], "a") == 0);
    destroy_str_list(list);

    s = "";
    list = str_split(s, ' ');
    assert(list[0] == NULL);
    destroy_str_list(list);

    s = "  ";
    list = str_split(s, ' ');
    assert(list[0] == NULL);
    destroy_str_list(list);
}

void test_str_strip() {
    char *s;

    s = str_strip("hello ");
    assert(strcmp(s, "hello") == 0);
    free(s);

    s = str_strip("hello");
    assert(strcmp(s, "hello") == 0);
    free(s);

    s = str_strip("\nhello ");
    assert(strcmp(s, "hello") == 0);
    free(s);

    s = str_strip("\nhello");
    assert(strcmp(s, "hello") == 0);
    free(s);

    s = str_strip("");
    assert(strcmp(s, "") == 0);
    free(s);

    s = str_strip("\n\t ");
    assert(strcmp(s, "") == 0);
    free(s);

    s = str_strip(" ");
    assert(strcmp(s, "") == 0);
    free(s);
}

void test_str_bulider() {
    str_builder_t sb;
    init_str_builder(&sb);

    str_builder_append(&sb, "%s", "hello");
    assert(sb.size == 5);
    assert(strcmp(sb.buf, "hello") == 0);
    assert(strlen(sb.buf) == 5);

    str_builder_append(&sb, "hello");
    assert(sb.size == 10);
    assert(strcmp(sb.buf, "hellohello") == 0);

    str_builder_append(&sb, "%c1", 'c');
    assert(sb.size == 12);
    assert(strcmp(sb.buf, "hellohelloc1") == 0);

    str_builder_append_char(&sb, 'x');
    assert(sb.size == 13);
    assert(strcmp(sb.buf, "hellohelloc1x") == 0);
}

int main() {
    printf("[TEST] str\n");

    test_str_split();
    test_str_strip();

    printf("[PASS] str\n");
    return 0;
}
