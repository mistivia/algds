#ifndef ALGDS_ARENA_H_
#define ALGDS_ARENA_H_

struct memblock {
    void *buf;
    int sz;
    int cap;
    struct memblock *next;
};
typedef struct memblock memblock_t;

struct arena {
    struct memblock head;
    struct memblock *current;
    int blocksz;
};
typedef struct arena arena_t;

void init_arena(arena_t *r, int blocksz);
void destroy_arena(arena_t *r);
void *arena_alloc(arena_t *r, int sz);

#endif
