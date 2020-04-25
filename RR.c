#include "utils.h"
#include "data_struct.h"

void RR_scheduler(Process proc_list[], int N)
{
        const int quantum = 500;
        int counter = 0;
        int left_jobs = N;
        int ready_p = 0;
        int running_p = -1;
        Queue *queue = (Queue*)calloc(1, sizeof(Queue));

        for (int time = 0; ; time++) {
                // start all process whose ready time <= time
                while (ready_p < N && proc_list[ready_p].ready_time <= time) {
                        proc_start(&proc_list[ready_p]);
                        queue_push(queue, ready_p);
                        ++ready_p;
                }

                // if CPU is available, assign a process to it
                if (running_p == -1) {
                        running_p = queue_pop(queue);
                        if (running_p != -1) {
                                proc_wakeup(&proc_list[running_p]);
                                counter = quantum;
                        }
                }

                TIME_UNIT;
                
                // if there is a process running, decrease its remaining time and counter
                if (running_p != -1) {
                        --proc_list[running_p].remaining_time;
                        --counter;
                        if (counter == 0 && proc_list[running_p].remaining_time != 0) {
                                proc_block(&proc_list[running_p]);
                                queue_push(queue, running_p);
                                running_p = -1;
                        }
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