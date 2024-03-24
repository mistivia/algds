#include "priority_queue.h"

#include <stdlib.h>
#include <string.h>

void init_priority_queue(priority_queue_t *pq, int cap, int elem_sz,
                         int (*cmp)(void *, void *)) {
    pq->cap = cap;
    pq->size = 0;
    pq->elemsz = elem_sz;
    pq->cmp = cmp;
    pq->buf = malloc(cap * elem_sz);
}

static void swap(priority_queue_t *pq, int a, int b) {
    char buf[pq->elemsz];
    void *tmp = buf;
    int elemsz = pq->elemsz;
    memcpy(tmp, pq->buf + a * elemsz, elemsz);
    memcpy(pq->buf + a * elemsz, pq->buf + b * elemsz, elemsz);
    memcpy(pq->buf + b * elemsz, tmp, elemsz);
}

static int cmp(priority_queue_t *pq, int a, int b) {
    return pq->cmp(pq->buf + a * pq->elemsz, pq->buf + b * pq->elemsz);
}

void priority_queue_push(priority_queue_t *pq, void *elem) {
    if (pq->size + 1 > pq->cap) {
        pq->buf = realloc(pq->buf, 2 * pq->cap * pq->elemsz);
        pq->cap *= 2;
    }
    memcpy(pq->buf + pq->size * pq->elemsz, elem, pq->elemsz);
    pq->size++;
    if (pq->size == 0) {
        return;
    }
    int i = pq->size - 1;
    while (i > 0 && cmp(pq, i, i / 2) > 0) {
        swap(pq, i, i / 2);
        i /= 2;
    }
}

static void heapify(priority_queue_t *pq, int idx) {
    int left, right, largest;
    left = 2 * idx + 1;
    right = 2 * idx + 2;
    if (left < pq->size && cmp(pq, left, idx) > 0) {
        largest = left;
    } else {
        largest = idx;
    }
    if (right < pq->size && cmp(pq, right, largest) > 0) {
        largest = right;
    }
    if (largest != idx) {
        swap(pq, largest, idx);
        heapify(pq, largest);
    }
}

void priority_queue_pop(priority_queue_t *pq) {
    if (pq->size == 0) return;
    memcpy(pq->buf, pq->buf + (pq->size - 1) * pq->elemsz, pq->elemsz);
    pq->size -= 1;
    heapify(pq, 0);
}

void *priority_queue_top(priority_queue_t *pq) {
    if (pq->size == 0) return NULL;
    return pq->buf;
}
