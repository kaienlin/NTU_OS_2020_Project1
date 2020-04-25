#ifndef _OSPJ1_DS_
#define _OSPJ1_DS_

#include <stdlib.h>
#include "utils.h"

typedef struct _List {
        int val;
        struct _List *next;
} List;

typedef struct _Queue {
        List *front, *back;
} Queue;

void queue_push(Queue *que, int val);
int queue_pop(Queue *que);

#endif
