#include "data_struct.h"

List* list_new(int val)
{
        List *li = (List*)malloc(sizeof(List));
        li->val = val;
        li->next = NULL;
        return li;
}

void queue_push(Queue *que, int val)
{
        if (que->back == NULL) {
                que->front = que->back = list_new(val);
        } else {
                List *new_node = list_new(val);
                que->back->next = new_node;
                que->back = new_node;
        }
}

int queue_pop(Queue *que)
{
        if (que->front == NULL) {
                return -1;
        } else {
                List *old = que->front;
                que->front = que->front->next;
                if (que->front == NULL)
                        que->back = NULL;
                int ret = old->val;
                free(old);
                return ret;
        }
}

void heap_push(Heap *heap, int val)
{       
        List *new = list_new(val);
        if (heap->head == NULL) {
                heap->head = new;
        } else {
                List *cur = heap->head, *prv = NULL;
                while (cur && val >= cur->val) {
                        prv = cur;
                        cur = cur->next;
                }
                if (prv == NULL)
                        new->next = cur, heap->head = new;
                else
                        prv->next = new, new->next = cur;
        }
}

int heap_pop(Heap *heap)
{
        if (heap->head == NULL)
                return -1;
        List *old = heap->head;
        int ret = old->val;
        heap->head = old->next;
        free(old);
        return ret;
}

int heap_top(Heap *heap)
{
        if (heap->head == NULL)
                return -1;
        return heap->head->val;
}
