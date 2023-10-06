// Copyright (C) 2023 Arcane Lactiflora <arcanelactiflora@outlook.com>
// Licensed under GPLv3. See LICENSE for details.

#ifndef ALGDS_STR_H_
#define ALGDS_STR_H_

#include <stdio.h>

char *str_strip(char *str);
char **str_split(char *str, char delim);
void destroy_str_list(char **list);

struct str_builder {
    char *buf;
    int size;
    int cap;
};
typedef struct str_builder str_builder_t;

void init_str_builder(str_builder_t *sb);
void str_builder_append(str_builder_t *sb, char *format, ...);
void str_builder_append_char(str_builder_t *sb, char c);

char *fgetline(FILE *fp);
int fpeek(FILE *fp);

#endif
