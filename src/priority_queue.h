// Copyright (C) 2023 Mistivia <mistivia@soverin.net>
// Licensed under GPLv3. See LICENSE for details.

#ifndef PQUEUE_H_
#define PQUEUE_H_

struct priority_queue {
    void *buf;
    int elemsz;
    int cap;
    int size;
    int (*cmp)(void *, void *);
};
typedef struct priority_queue priority_queue_t;

void init_priority_queue(priority_queue_t *pq, int cap, int elemsz,
                         int (*cmp)(void *, void *));
void priority_queue_push(priority_queue_t *pq, void *elem);
void priority_queue_pop(priority_queue_t *pq);
void *priority_queue_top();

#endif
