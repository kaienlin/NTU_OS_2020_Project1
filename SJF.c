#include "utils.h"
#include "data_struct.h"

void SJF_scheduler(Process proc_list[], int N)
{       
        int left_jobs = N;
        int ready_p = 0;
        int running_p = -1;
        Heap *heap = (Heap*)calloc(1, sizeof(Heap));

        for (int time = 0; ; time++) {
                // start all process whose ready time <= time
                while (ready_p < N && proc_list[ready_p].ready_time <= time) {
                        proc_start(&proc_list[ready_p]);
                        heap_push(heap, ready_p);
                        ++ready_p;
                }

                // if CPU is available, assign a process to it
                if (running_p == -1) {
                        running_p = heap_pop(heap);
                        if (running_p != -1) {
                                proc_wakeup(&proc_list[running_p]);
                        }
                }

                TIME_UNIT;

                // if there is process running, decrease its remaining time
                if (running_p != -1) {
                        --proc_list[running_p].remaining_time;
                        if (proc_list[running_p].remaining_time == 0) {
                                proc_term(&proc_list[running_p]);
                                running_p = -1;
                                --left_jobs;
                                if (left_jobs == 0)
                                        exit(EXIT_SUCCESS);
                        }
                }
        }
}