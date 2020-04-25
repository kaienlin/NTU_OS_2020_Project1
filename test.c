#include "data_struct.h"
#include <stdio.h>

int main()
{
        Heap *heap = (Heap*)calloc(1, sizeof(Heap));
        while (1) {
                int cmd; scanf("%d", &cmd);
                if (cmd == 1) {
                        int x; scanf("%d", &x);
                        heap_push(heap, x);
                } else if (cmd == 2) {
                        int v = heap_pop(heap);
                        printf("pop: %d\n", v);
                }
        }
}