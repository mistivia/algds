#include "arena.h"

#include <stdlib.h>

void init_arena(arena_t *r, int blocksz) {
    if (blocksz < 4096) blocksz = 4096;
    r->head = (struct memblock){NULL, 0, blocksz};
    r->head.buf = malloc(blocksz);
    r->current = &(r->head);
    r->blocksz = blocksz;
}

void destroy_arena(arena_t *r) {
    free(r->head.buf);
    struct memblock *cur = r->head.next;
    while (cur != NULL) {
        struct memblock *prev = cur;
        cur = cur->next;
        free(prev->buf);
        free(prev);
    }
}

void *arena_alloc(arena_t *r, int sz) {
    int remain = r->current->cap - r->current->sz;
    if (remain >= sz) {
        void *ptr = r->current->buf + r->current->sz;
        r->current->sz += sz;
        return ptr;
    }
    int blocksz = sz > blocksz ? sz : blocksz;
    struct memblock *nextblock = malloc(sizeof(struct memblock));
    void *ptr = malloc(blocksz);
    nextblock->buf = ptr;
    nextblock->cap = blocksz;
    nextblock->sz = sz;
    r->current->next = nextblock;
    r->current = nextblock;
    return ptr;
}
