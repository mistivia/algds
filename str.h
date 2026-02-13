#ifndef ALGDS_STR_H_
#define ALGDS_STR_H_

#include <stdio.h>

char *str_strip(char *str);
char **str_split(char *str, char delim);
void destroy_str_list(char **list);

typedef struct {
    char *buf;
    int size;
    int cap;
} str_builder_t;

str_builder_t str_builder_create();
void str_builder_append(str_builder_t *self, char *format, ...);
void str_builder_append_char(str_builder_t *self, char c);
str_builder_t str_builder_move(str_builder_t* self);
void str_builder_destroy(str_builder_t* self);


char *fgetline(FILE *fp);
int fpeek(FILE *fp);

#endif
