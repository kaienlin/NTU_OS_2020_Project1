#include "data_struct.h"
#include "utils.h"

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
