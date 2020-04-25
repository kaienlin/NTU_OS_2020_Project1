#ifndef _OSPJ1_DS_
#define _OSPJ1_DS_

#include <stdlib.h>

typedef struct _List {
        int val;
        struct _List *next;
} List;

typedef struct _Queue {
        List *front, *back;
} Queue;

void queue_push(Queue *que, int val);
int queue_pop(Queue *que);

typedef struct _Heap {
        List *head;
} Heap;

void heap_push(Heap *heap, int val);
int heap_pop(Heap *heap);
int heap_top(Heap *heap);

#endif
